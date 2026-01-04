"""OSC sequence generation for yetty plugins.

New OSC Format: ESC ] 123456;<generic-args>;<plugin-args>;<payload> ST
Where ST = ESC \\ (string terminator)

Generic args (Unix-like command line):
  - create -x X -y Y -w W -h H -p PLUGIN [-r]
  - ls [--all]
  - plugins
  - kill --id ID | --plugin NAME
  - stop --id ID | --plugin NAME
  - start --id ID | --plugin NAME
  - update --id ID

When running inside tmux, sequences are wrapped in DCS passthrough:
  ESC P tmux; <escaped_content> ESC \\
Where ESC characters in content are doubled (ESC -> ESC ESC).
"""

import os
from . import base94

VENDOR_ID = 999999


def is_inside_tmux() -> bool:
    """Check if running inside tmux."""
    return 'TMUX' in os.environ


def wrap_for_tmux(sequence: str) -> str:
    """Wrap an escape sequence for tmux DCS passthrough."""
    escaped = sequence.replace('\033', '\033\033')
    return f"\033Ptmux;{escaped}\033\\"


def maybe_wrap_for_tmux(sequence: str) -> str:
    """Wrap sequence for tmux passthrough if running inside tmux."""
    if is_inside_tmux():
        return wrap_for_tmux(sequence)
    return sequence


def create_sequence(
    plugin: str,
    x: int = 0,
    y: int = 0,
    w: int = 0,
    h: int = 0,
    relative: bool = True,
    payload: str = "",
    plugin_args: str = ""
) -> str:
    """Create an OSC sequence for plugin creation.

    Args:
        plugin: Plugin name (e.g., 'image', 'shader', 'markdown')
        x, y: Position in cells
        w, h: Size in cells (0 = stretch to edge)
        relative: If True, position relative to cursor; else absolute
        payload: Raw payload string (will be base94 encoded)
        plugin_args: Plugin-specific args (passed as-is)

    Returns:
        Complete OSC escape sequence
    """
    args = f"create -p {plugin} -x {x} -y {y} -w {w} -h {h}"
    if relative:
        args += " -r"

    encoded_payload = base94.encode_string(payload) if payload else ""
    return f"\033]{VENDOR_ID};{args};{plugin_args};{encoded_payload}\033\\"


def create_sequence_bytes(
    plugin: str,
    x: int = 0,
    y: int = 0,
    w: int = 0,
    h: int = 0,
    relative: bool = True,
    payload_bytes: bytes = b"",
    plugin_args: str = ""
) -> str:
    """Create an OSC sequence with binary payload."""
    args = f"create -p {plugin} -x {x} -y {y} -w {w} -h {h}"
    if relative:
        args += " -r"

    encoded_payload = base94.encode(payload_bytes) if payload_bytes else ""
    return f"\033]{VENDOR_ID};{args};{plugin_args};{encoded_payload}\033\\"


def list_sequence(all: bool = False) -> str:
    """Create an OSC sequence to list active layers."""
    args = "ls --all" if all else "ls"
    return f"\033]{VENDOR_ID};{args};;\033\\"


def plugins_sequence() -> str:
    """Create an OSC sequence to list available plugins."""
    return f"\033]{VENDOR_ID};plugins;;\033\\"


def kill_sequence(id: str = None, plugin: str = None) -> str:
    """Create an OSC sequence to kill layer(s)."""
    if id:
        args = f"kill --id {id}"
    elif plugin:
        args = f"kill --plugin {plugin}"
    else:
        raise ValueError("Either id or plugin must be specified")
    return f"\033]{VENDOR_ID};{args};;\033\\"


def stop_sequence(id: str = None, plugin: str = None) -> str:
    """Create an OSC sequence to stop layer(s)."""
    if id:
        args = f"stop --id {id}"
    elif plugin:
        args = f"stop --plugin {plugin}"
    else:
        raise ValueError("Either id or plugin must be specified")
    return f"\033]{VENDOR_ID};{args};;\033\\"


def start_sequence(id: str = None, plugin: str = None) -> str:
    """Create an OSC sequence to start layer(s)."""
    if id:
        args = f"start --id {id}"
    elif plugin:
        args = f"start --plugin {plugin}"
    else:
        raise ValueError("Either id or plugin must be specified")
    return f"\033]{VENDOR_ID};{args};;\033\\"


def update_sequence(id: str, payload: str = "", plugin_args: str = "") -> str:
    """Create an OSC sequence to update a layer."""
    args = f"update --id {id}"
    encoded_payload = base94.encode_string(payload) if payload else ""
    return f"\033]{VENDOR_ID};{args};{plugin_args};{encoded_payload}\033\\"
