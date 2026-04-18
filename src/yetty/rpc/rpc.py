#!/usr/bin/env -S uv run
# /// script
# requires-python = ">=3.11"
# dependencies = ["msgpack", "click"]
# ///
"""
Async RPC client for yetty terminal.

Protocol: msgpack-rpc over Unix domain sockets
- Notification: [2, channel, method, params]
- Request:      [0, msgid, channel, method, params]
- Response:     [1, msgid, error, result]

Channels:
- 0: EventLoop (keyboard, mouse, window events)
- 1: CardStream (card buffer/texture streaming)
"""

from __future__ import annotations

import asyncio
import os
import struct
from dataclasses import dataclass
from enum import IntEnum
from typing import Any

import msgpack


class Channel(IntEnum):
    EventLoop = 0
    CardStream = 1


class MessageType(IntEnum):
    Request = 0
    Response = 1
    Notification = 2


@dataclass
class RpcResponse:
    msgid: int
    error: Any
    result: Any


class RpcClient:
    """Async RPC client for yetty terminal."""

    def __init__(self, socket_path: str | None = None):
        self.socket_path = socket_path or os.environ.get("YETTY_SOCKET", "")
        self._reader: asyncio.StreamReader | None = None
        self._writer: asyncio.StreamWriter | None = None
        self._msgid = 0
        self._pending: dict[int, asyncio.Future[RpcResponse]] = {}
        self._recv_task: asyncio.Task | None = None
        self._unpacker = msgpack.Unpacker(raw=False)
        self._connected = False

    async def connect(self) -> None:
        """Connect to the yetty RPC server."""
        if not self.socket_path:
            raise ValueError("No socket path specified. Set YETTY_SOCKET or pass socket_path.")
        self._reader, self._writer = await asyncio.open_unix_connection(self.socket_path)
        self._connected = True
        self._recv_task = asyncio.create_task(self._recv_loop())

    async def disconnect(self) -> None:
        """Disconnect from the server."""
        if self._recv_task:
            self._recv_task.cancel()
            try:
                await self._recv_task
            except asyncio.CancelledError:
                pass
            self._recv_task = None
        if self._writer:
            self._writer.close()
            await self._writer.wait_closed()
            self._writer = None
            self._reader = None
        self._connected = False

    async def __aenter__(self) -> RpcClient:
        await self.connect()
        return self

    async def __aexit__(self, *args) -> None:
        await self.disconnect()

    def _next_msgid(self) -> int:
        self._msgid += 1
        return self._msgid

    async def _recv_loop(self) -> None:
        """Background task to receive and dispatch responses."""
        assert self._reader is not None
        try:
            while True:
                data = await self._reader.read(65536)
                if not data:
                    break
                self._unpacker.feed(data)
                for msg in self._unpacker:
                    self._handle_message(msg)
        except asyncio.CancelledError:
            pass
        except Exception as e:
            # Cancel all pending futures with error
            for fut in self._pending.values():
                if not fut.done():
                    fut.set_exception(e)
            self._pending.clear()

    def _handle_message(self, msg: list) -> None:
        """Handle an incoming message."""
        if not isinstance(msg, list) or len(msg) < 3:
            return
        msg_type = msg[0]
        if msg_type == MessageType.Response:
            # [1, msgid, error, result]
            if len(msg) < 4:
                return
            msgid = msg[1]
            error = msg[2]
            result = msg[3]
            fut = self._pending.pop(msgid, None)
            if fut and not fut.done():
                fut.set_result(RpcResponse(msgid, error, result))

    async def notify(self, channel: Channel, method: str, params: dict) -> None:
        """Send a notification (no response expected)."""
        if not self._connected:
            raise ConnectionError("Not connected")
        # [2, channel, method, params]
        msg = [MessageType.Notification, int(channel), method, params]
        data = msgpack.packb(msg, use_bin_type=True)
        assert self._writer is not None
        self._writer.write(data)
        await self._writer.drain()

    async def request(self, channel: Channel, method: str, params: dict) -> Any:
        """Send a request and wait for response."""
        if not self._connected:
            raise ConnectionError("Not connected")
        msgid = self._next_msgid()
        # [0, msgid, channel, method, params]
        msg = [MessageType.Request, msgid, int(channel), method, params]
        data = msgpack.packb(msg, use_bin_type=True)

        fut: asyncio.Future[RpcResponse] = asyncio.get_event_loop().create_future()
        self._pending[msgid] = fut

        assert self._writer is not None
        self._writer.write(data)
        await self._writer.drain()

        response = await fut
        if response.error is not None:
            raise RuntimeError(f"RPC error: {response.error}")
        return response.result

    # ─── EventLoop Channel Methods ───────────────────────────────────────────

    async def key_down(self, key: int, mods: int = 0, scancode: int = 0) -> None:
        """Send key down event."""
        await self.notify(Channel.EventLoop, "key_down", {
            "key": key, "mods": mods, "scancode": scancode
        })

    async def key_up(self, key: int, mods: int = 0, scancode: int = 0) -> None:
        """Send key up event."""
        await self.notify(Channel.EventLoop, "key_up", {
            "key": key, "mods": mods, "scancode": scancode
        })

    async def char_input(self, codepoint: int, mods: int = 0) -> None:
        """Send character input event."""
        await self.notify(Channel.EventLoop, "char", {
            "codepoint": codepoint, "mods": mods
        })

    async def mouse_down(self, x: float, y: float, button: int) -> None:
        """Send mouse button down event."""
        await self.notify(Channel.EventLoop, "mouse_down", {
            "x": x, "y": y, "button": button
        })

    async def mouse_up(self, x: float, y: float, button: int) -> None:
        """Send mouse button up event."""
        await self.notify(Channel.EventLoop, "mouse_up", {
            "x": x, "y": y, "button": button
        })

    async def mouse_move(self, x: float, y: float) -> None:
        """Send mouse move event."""
        await self.notify(Channel.EventLoop, "mouse_move", {"x": x, "y": y})

    async def mouse_drag(self, x: float, y: float, button: int) -> None:
        """Send mouse drag event."""
        await self.notify(Channel.EventLoop, "mouse_drag", {
            "x": x, "y": y, "button": button
        })

    async def scroll(self, x: float, y: float, dx: float, dy: float, mods: int = 0) -> None:
        """Send scroll event."""
        await self.notify(Channel.EventLoop, "scroll", {
            "x": x, "y": y, "dx": dx, "dy": dy, "mods": mods
        })

    async def set_focus(self, object_id: int) -> None:
        """Set focus to an object."""
        await self.notify(Channel.EventLoop, "set_focus", {"object_id": object_id})

    async def resize(self, width: float, height: float) -> None:
        """Resize the window."""
        await self.notify(Channel.EventLoop, "resize", {
            "width": width, "height": height
        })

    async def context_menu_action(self, object_id: int, action: str, row: int, col: int) -> None:
        """Trigger a context menu action."""
        await self.notify(Channel.EventLoop, "context_menu_action", {
            "object_id": object_id, "action": action, "row": row, "col": col
        })

    async def card_mouse_down(self, target_id: int, x: float, y: float, button: int) -> None:
        """Send mouse down event to a card."""
        await self.notify(Channel.EventLoop, "card_mouse_down", {
            "target_id": target_id, "x": x, "y": y, "button": button
        })

    async def card_mouse_up(self, target_id: int, x: float, y: float, button: int) -> None:
        """Send mouse up event to a card."""
        await self.notify(Channel.EventLoop, "card_mouse_up", {
            "target_id": target_id, "x": x, "y": y, "button": button
        })

    async def card_mouse_move(self, target_id: int, x: float, y: float) -> None:
        """Send mouse move event to a card."""
        await self.notify(Channel.EventLoop, "card_mouse_move", {
            "target_id": target_id, "x": x, "y": y
        })

    async def card_scroll(self, target_id: int, x: float, y: float, dx: float, dy: float) -> None:
        """Send scroll event to a card."""
        await self.notify(Channel.EventLoop, "card_scroll", {
            "target_id": target_id, "x": x, "y": y, "dx": dx, "dy": dy
        })

    async def close(self, object_id: int) -> None:
        """Close an object (request, waits for response)."""
        await self.request(Channel.EventLoop, "close", {"object_id": object_id})

    async def split(self, object_id: int, orientation: int) -> None:
        """Split a view (request, waits for response)."""
        await self.request(Channel.EventLoop, "split", {
            "object_id": object_id, "orientation": orientation
        })

    async def ui_tree(self) -> str:
        """Get UI tree (request, returns JSON string)."""
        return await self.request(Channel.EventLoop, "ui_tree", {})

    # ─── High-Level Commands ─────────────────────────────────────────────────

    async def type_text(self, text: str, delay: float = 0.01) -> None:
        """Type text by sending character events for each character."""
        for char in text:
            await self.char_input(ord(char))
            if delay > 0:
                await asyncio.sleep(delay)

    async def run_command(self, command: str, delay: float = 0.01) -> None:
        """Type a command and press Enter."""
        await self.type_text(command, delay)
        # Press Enter (key code 257 in GLFW, but we use char input for '\n')
        await self.char_input(ord('\n'))

    async def press_key(self, key: int, mods: int = 0) -> None:
        """Press and release a key."""
        await self.key_down(key, mods)
        await self.key_up(key, mods)


# ─── GLFW Key Codes (common ones) ────────────────────────────────────────────

class Keys:
    """GLFW key codes for common keys."""
    SPACE = 32
    APOSTROPHE = 39
    COMMA = 44
    MINUS = 45
    PERIOD = 46
    SLASH = 47

    # 0-9 are 48-57
    NUM_0 = 48
    NUM_9 = 57

    SEMICOLON = 59
    EQUAL = 61

    # A-Z are 65-90
    A = 65
    Z = 90

    LEFT_BRACKET = 91
    BACKSLASH = 92
    RIGHT_BRACKET = 93
    GRAVE_ACCENT = 96

    ESCAPE = 256
    ENTER = 257
    TAB = 258
    BACKSPACE = 259
    INSERT = 260
    DELETE = 261
    RIGHT = 262
    LEFT = 263
    DOWN = 264
    UP = 265
    PAGE_UP = 266
    PAGE_DOWN = 267
    HOME = 268
    END = 269
    CAPS_LOCK = 280
    SCROLL_LOCK = 281
    NUM_LOCK = 282
    PRINT_SCREEN = 283
    PAUSE = 284

    F1 = 290
    F2 = 291
    F3 = 292
    F4 = 293
    F5 = 294
    F6 = 295
    F7 = 296
    F8 = 297
    F9 = 298
    F10 = 299
    F11 = 300
    F12 = 301

    LEFT_SHIFT = 340
    LEFT_CONTROL = 341
    LEFT_ALT = 342
    LEFT_SUPER = 343
    RIGHT_SHIFT = 344
    RIGHT_CONTROL = 345
    RIGHT_ALT = 346
    RIGHT_SUPER = 347


class Mods:
    """GLFW modifier key flags."""
    SHIFT = 0x0001
    CONTROL = 0x0002
    ALT = 0x0004
    SUPER = 0x0008
    CAPS_LOCK = 0x0010
    NUM_LOCK = 0x0020


# ─── CLI ─────────────────────────────────────────────────────────────────────

def run_async(coro):
    """Run an async coroutine from sync context."""
    return asyncio.get_event_loop().run_until_complete(coro)


async def get_client(socket: str | None) -> RpcClient:
    """Create and connect a client."""
    client = RpcClient(socket)
    await client.connect()
    return client


# ─── Click CLI ───────────────────────────────────────────────────────────────

try:
    import click
except ImportError:
    click = None  # type: ignore

if click:
    @click.group()
    @click.option("--socket", "-s", envvar="YETTY_SOCKET", help="Socket path (or YETTY_SOCKET env)")
    @click.pass_context
    def cli(ctx, socket):
        """Yetty RPC client CLI."""
        ctx.ensure_object(dict)
        ctx.obj["socket"] = socket

    # ─── Primitive Event Commands ────────────────────────────────────────────

    @cli.command()
    @click.argument("key", type=int)
    @click.option("--mods", "-m", default=0, type=int, help="Modifier flags")
    @click.pass_context
    def key_down(ctx, key, mods):
        """Send key down event. KEY is GLFW key code."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.key_down(key, mods)
        run_async(_run())

    @cli.command()
    @click.argument("key", type=int)
    @click.option("--mods", "-m", default=0, type=int, help="Modifier flags")
    @click.pass_context
    def key_up(ctx, key, mods):
        """Send key up event. KEY is GLFW key code."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.key_up(key, mods)
        run_async(_run())

    @cli.command()
    @click.argument("key", type=int)
    @click.option("--mods", "-m", default=0, type=int, help="Modifier flags")
    @click.pass_context
    def press(ctx, key, mods):
        """Press and release a key. KEY is GLFW key code."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.press_key(key, mods)
        run_async(_run())

    @cli.command("char")
    @click.argument("codepoint", type=int)
    @click.option("--mods", "-m", default=0, type=int, help="Modifier flags")
    @click.pass_context
    def char_cmd(ctx, codepoint, mods):
        """Send character input event. CODEPOINT is Unicode code point."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.char_input(codepoint, mods)
        run_async(_run())

    @cli.command()
    @click.argument("x", type=float)
    @click.argument("y", type=float)
    @click.argument("button", type=int, default=0)
    @click.pass_context
    def mouse_down(ctx, x, y, button):
        """Send mouse button down event."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.mouse_down(x, y, button)
        run_async(_run())

    @cli.command()
    @click.argument("x", type=float)
    @click.argument("y", type=float)
    @click.argument("button", type=int, default=0)
    @click.pass_context
    def mouse_up(ctx, x, y, button):
        """Send mouse button up event."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.mouse_up(x, y, button)
        run_async(_run())

    @cli.command()
    @click.argument("x", type=float)
    @click.argument("y", type=float)
    @click.pass_context
    def mouse_move(ctx, x, y):
        """Send mouse move event."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.mouse_move(x, y)
        run_async(_run())

    @cli.command()
    @click.argument("x", type=float)
    @click.argument("y", type=float)
    @click.argument("dx", type=float)
    @click.argument("dy", type=float)
    @click.option("--mods", "-m", default=0, type=int, help="Modifier flags")
    @click.pass_context
    def scroll(ctx, x, y, dx, dy, mods):
        """Send scroll event at position (X, Y) with delta (DX, DY)."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.scroll(x, y, dx, dy, mods)
        run_async(_run())

    @cli.command()
    @click.argument("width", type=float)
    @click.argument("height", type=float)
    @click.pass_context
    def resize(ctx, width, height):
        """Resize the terminal window."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.resize(width, height)
        run_async(_run())

    # ─── Query Commands ──────────────────────────────────────────────────────

    @cli.command()
    @click.pass_context
    def ui_tree(ctx):
        """Get UI tree as JSON."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                tree = await client.ui_tree()
                click.echo(tree)
        run_async(_run())

    # ─── High-Level Commands ─────────────────────────────────────────────────

    @cli.command("type")
    @click.argument("text")
    @click.option("--delay", "-d", default=0.01, type=float, help="Delay between chars (seconds)")
    @click.pass_context
    def type_cmd(ctx, text, delay):
        """Type text character by character."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.type_text(text, delay)
        run_async(_run())

    @cli.command()
    @click.argument("command")
    @click.option("--delay", "-d", default=0.01, type=float, help="Delay between chars (seconds)")
    @click.pass_context
    def run(ctx, command, delay):
        """Type a shell command and press Enter."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.run_command(command, delay)
        run_async(_run())

    @cli.command()
    @click.pass_context
    def enter(ctx):
        """Press Enter key."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.char_input(ord('\n'))
        run_async(_run())

    @cli.command()
    @click.pass_context
    def tab(ctx):
        """Press Tab key."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.press_key(Keys.TAB)
        run_async(_run())

    @cli.command()
    @click.pass_context
    def escape(ctx):
        """Press Escape key."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.press_key(Keys.ESCAPE)
        run_async(_run())

    @cli.command("ctrl-c")
    @click.pass_context
    def ctrl_c(ctx):
        """Send Ctrl+C (interrupt)."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                # Send Ctrl+C as char input (code 3)
                await client.char_input(3, Mods.CONTROL)
        run_async(_run())

    @cli.command("ctrl-d")
    @click.pass_context
    def ctrl_d(ctx):
        """Send Ctrl+D (EOF)."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.char_input(4, Mods.CONTROL)
        run_async(_run())

    @cli.command("ctrl-z")
    @click.pass_context
    def ctrl_z(ctx):
        """Send Ctrl+Z (suspend)."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.char_input(26, Mods.CONTROL)
        run_async(_run())

    @cli.command()
    @click.argument("object_id", type=int)
    @click.pass_context
    def focus(ctx, object_id):
        """Set focus to an object by ID."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.set_focus(object_id)
        run_async(_run())

    @cli.command()
    @click.argument("object_id", type=int)
    @click.pass_context
    def close(ctx, object_id):
        """Close an object by ID."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                await client.close(object_id)
        run_async(_run())

    @cli.command()
    @click.argument("object_id", type=int)
    @click.option("--horizontal", "-h", is_flag=True, help="Split horizontally")
    @click.pass_context
    def split(ctx, object_id, horizontal):
        """Split a view. Default is vertical split."""
        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                orientation = 1 if horizontal else 0
                await client.split(object_id, orientation)
        run_async(_run())

    # ─── Interactive Mode ────────────────────────────────────────────────────

    @cli.command()
    @click.pass_context
    def interactive(ctx):
        """Interactive mode: type directly, each line is sent to terminal."""
        import sys

        async def _run():
            async with RpcClient(ctx.obj["socket"]) as client:
                click.echo("Interactive mode. Type commands, press Enter to send.")
                click.echo("Ctrl+D to exit, Ctrl+C sends interrupt to terminal.")
                click.echo()

                while True:
                    try:
                        line = input("> ")
                        await client.run_command(line, delay=0.005)
                    except EOFError:
                        click.echo("\nExiting.")
                        break
                    except KeyboardInterrupt:
                        await client.char_input(3, Mods.CONTROL)
                        click.echo("^C")

        run_async(_run())

    # ─── Key Code Reference ──────────────────────────────────────────────────

    @cli.command()
    def keys():
        """Show common GLFW key codes."""
        click.echo("Common GLFW Key Codes:")
        click.echo()
        click.echo("Letters:      A-Z = 65-90")
        click.echo("Numbers:      0-9 = 48-57")
        click.echo()
        click.echo("Special keys:")
        click.echo(f"  ENTER       = {Keys.ENTER}")
        click.echo(f"  TAB         = {Keys.TAB}")
        click.echo(f"  BACKSPACE   = {Keys.BACKSPACE}")
        click.echo(f"  ESCAPE      = {Keys.ESCAPE}")
        click.echo(f"  SPACE       = {Keys.SPACE}")
        click.echo(f"  DELETE      = {Keys.DELETE}")
        click.echo()
        click.echo("Arrow keys:")
        click.echo(f"  LEFT        = {Keys.LEFT}")
        click.echo(f"  RIGHT       = {Keys.RIGHT}")
        click.echo(f"  UP          = {Keys.UP}")
        click.echo(f"  DOWN        = {Keys.DOWN}")
        click.echo()
        click.echo("Navigation:")
        click.echo(f"  HOME        = {Keys.HOME}")
        click.echo(f"  END         = {Keys.END}")
        click.echo(f"  PAGE_UP     = {Keys.PAGE_UP}")
        click.echo(f"  PAGE_DOWN   = {Keys.PAGE_DOWN}")
        click.echo()
        click.echo("Function keys: F1-F12 = 290-301")
        click.echo()
        click.echo("Modifier flags (--mods/-m):")
        click.echo(f"  SHIFT       = {Mods.SHIFT}")
        click.echo(f"  CONTROL     = {Mods.CONTROL}")
        click.echo(f"  ALT         = {Mods.ALT}")
        click.echo(f"  SUPER       = {Mods.SUPER}")

    def main():
        cli()

else:
    def main():
        print("Click library not installed. Install with: pip install click")
        print("Or use the RpcClient class directly in Python.")


if __name__ == "__main__":
    main()
