#!/bin/bash
# WebSocket proxy for Yetty VNC
# Bridges WebSocket connections from browser to VNC server TCP socket

set -e

WS_PORT="${WS_PORT:-5901}"
VNC_PORT="${VNC_PORT:-5900}"
VNC_HOST="${VNC_HOST:-127.0.0.1}"

usage() {
    echo "Usage: $0 [HOST:PORT] [WS_PORT]"
    echo "       $0 [options]"
    echo ""
    echo "Sets up a WebSocket-to-TCP proxy for Yetty VNC."
    echo "The browser VNC client connects via WebSocket, this proxy"
    echo "forwards to the VNC server's TCP socket."
    echo ""
    echo "Positional arguments:"
    echo "  HOST:PORT    VNC server address (default: 127.0.0.1:5900)"
    echo "  WS_PORT      WebSocket listen port (default: 5901)"
    echo ""
    echo "Options:"
    echo "  -w, --ws-port PORT    WebSocket listen port (default: 5901)"
    echo "  -v, --vnc-port PORT   VNC server TCP port (default: 5900)"
    echo "  -H, --vnc-host HOST   VNC server host (default: 127.0.0.1)"
    echo "  -h, --help            Show this help"
    echo ""
    echo "Environment variables:"
    echo "  WS_PORT   WebSocket listen port"
    echo "  VNC_PORT  VNC server TCP port"
    echo "  VNC_HOST  VNC server host"
    echo ""
    echo "Examples:"
    echo "  # Default: proxy localhost:5900 -> ws://0.0.0.0:5901"
    echo "  $0"
    echo ""
    echo "  # Proxy to remote VNC server"
    echo "  $0 192.168.1.100:5900"
    echo ""
    echo "  # Proxy with custom WebSocket port"
    echo "  $0 192.168.1.100:5900 8080"
    echo ""
    echo "  # Using named options"
    echo "  $0 --vnc-host 192.168.1.100 --vnc-port 5900 --ws-port 8080"
    exit 0
}

# Parse arguments - support both positional and named options
# Positional: [HOST:PORT] [WS_PORT]
if [[ $# -gt 0 && ! "$1" =~ ^- ]]; then
    # First positional arg: HOST:PORT
    if [[ "$1" =~ : ]]; then
        VNC_HOST="${1%:*}"
        VNC_PORT="${1#*:}"
    else
        VNC_HOST="$1"
    fi
    shift
    # Second positional arg: WS_PORT
    if [[ $# -gt 0 && ! "$1" =~ ^- ]]; then
        WS_PORT="$1"
        shift
    fi
fi

while [[ $# -gt 0 ]]; do
    case $1 in
        -w|--ws-port)
            WS_PORT="$2"
            shift 2
            ;;
        -v|--vnc-port)
            VNC_PORT="$2"
            shift 2
            ;;
        -H|--vnc-host)
            VNC_HOST="$2"
            shift 2
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

echo "Yetty VNC WebSocket Proxy"
echo "========================="
echo "WebSocket: ws://0.0.0.0:$WS_PORT"
echo "VNC Server: $VNC_HOST:$VNC_PORT"
echo ""

# Try websocat first (lightweight, Rust-based)
if command -v websocat &> /dev/null; then
    echo "Using websocat..."
    exec websocat --binary ws-l:0.0.0.0:$WS_PORT tcp:$VNC_HOST:$VNC_PORT
fi

# Try websockify (Python, common with noVNC)
if command -v websockify &> /dev/null; then
    echo "Using websockify..."
    exec websockify 0.0.0.0:$WS_PORT $VNC_HOST:$VNC_PORT
fi

# Try socat with some creativity (if no dedicated WS tool)
# Note: This won't work - socat doesn't speak WebSocket protocol
# Keeping as placeholder to show what's needed

# Try Node.js inline script
if command -v node &> /dev/null; then
    echo "Using Node.js WebSocket proxy..."
    node -e "
const http = require('http');
const net = require('net');
const crypto = require('crypto');

const server = http.createServer();
server.on('upgrade', (req, socket, head) => {
    const key = req.headers['sec-websocket-key'];
    const accept = crypto.createHash('sha1')
        .update(key + '258EAFA5-E914-47DA-95CA-C5AB0DC85B11')
        .digest('base64');

    socket.write([
        'HTTP/1.1 101 Switching Protocols',
        'Upgrade: websocket',
        'Connection: Upgrade',
        'Sec-WebSocket-Accept: ' + accept,
        '', ''
    ].join('\r\n'));

    const vnc = net.connect($VNC_PORT, '$VNC_HOST');

    // WebSocket frame parser state
    let buffer = Buffer.alloc(0);

    socket.on('data', (data) => {
        buffer = Buffer.concat([buffer, data]);
        while (buffer.length >= 2) {
            const fin = (buffer[0] & 0x80) !== 0;
            const opcode = buffer[0] & 0x0f;
            const masked = (buffer[1] & 0x80) !== 0;
            let payloadLen = buffer[1] & 0x7f;
            let offset = 2;

            if (payloadLen === 126) {
                if (buffer.length < 4) return;
                payloadLen = buffer.readUInt16BE(2);
                offset = 4;
            } else if (payloadLen === 127) {
                if (buffer.length < 10) return;
                payloadLen = Number(buffer.readBigUInt64BE(2));
                offset = 10;
            }

            const maskOffset = offset;
            if (masked) offset += 4;

            if (buffer.length < offset + payloadLen) return;

            let payload = buffer.slice(offset, offset + payloadLen);
            if (masked) {
                const mask = buffer.slice(maskOffset, maskOffset + 4);
                for (let i = 0; i < payload.length; i++) {
                    payload[i] ^= mask[i % 4];
                }
            }

            if (opcode === 0x02) { // Binary frame
                vnc.write(payload);
            } else if (opcode === 0x08) { // Close
                socket.end();
                vnc.end();
                return;
            }

            buffer = buffer.slice(offset + payloadLen);
        }
    });

    vnc.on('data', (data) => {
        // Send as binary WebSocket frame
        const len = data.length;
        let header;
        if (len < 126) {
            header = Buffer.from([0x82, len]);
        } else if (len < 65536) {
            header = Buffer.from([0x82, 126, (len >> 8) & 0xff, len & 0xff]);
        } else {
            header = Buffer.alloc(10);
            header[0] = 0x82;
            header[1] = 127;
            header.writeBigUInt64BE(BigInt(len), 2);
        }
        socket.write(Buffer.concat([header, data]));
    });

    vnc.on('close', () => socket.end());
    vnc.on('error', (e) => { console.error('VNC error:', e.message); socket.end(); });
    socket.on('close', () => vnc.end());
    socket.on('error', (e) => { console.error('WS error:', e.message); vnc.end(); });
});

server.listen($WS_PORT, '0.0.0.0', () => {
    console.log('WebSocket proxy ready');
});
"
    exit 0
fi

echo "Error: No WebSocket proxy tool found!"
echo ""
echo "Please install one of:"
echo "  - websocat: cargo install websocat"
echo "  - websockify: pip install websockify"
echo "  - Node.js: apt install nodejs (or brew install node)"
exit 1
