#!/bin/bash
# WebSocket proxy for Telnet daemon
# Bridges WebSocket connections from browser to telnet server TCP socket

set -e

WS_PORT="${WS_PORT:-8024}"
TELNET_PORT="${TELNET_PORT:-8023}"
TELNET_HOST="${TELNET_HOST:-127.0.0.1}"

usage() {
    echo "Usage: $0 [HOST:PORT] [WS_PORT]"
    echo "       $0 [options]"
    echo ""
    echo "Sets up a WebSocket-to-TCP proxy for telnet."
    echo "The browser connects via WebSocket, this proxy"
    echo "forwards to the telnet daemon's TCP socket."
    echo ""
    echo "Positional arguments:"
    echo "  HOST:PORT    Telnet server address (default: 127.0.0.1:8023)"
    echo "  WS_PORT      WebSocket listen port (default: 8024)"
    echo ""
    echo "Options:"
    echo "  -w, --ws-port PORT      WebSocket listen port (default: 8024)"
    echo "  -t, --telnet-port PORT  Telnet server TCP port (default: 8023)"
    echo "  -H, --telnet-host HOST  Telnet server host (default: 127.0.0.1)"
    echo "  -h, --help              Show this help"
    echo ""
    echo "Environment variables:"
    echo "  WS_PORT      WebSocket listen port"
    echo "  TELNET_PORT  Telnet server TCP port"
    echo "  TELNET_HOST  Telnet server host"
    echo ""
    echo "Examples:"
    echo "  # Default: proxy localhost:8023 -> ws://0.0.0.0:8024"
    echo "  $0"
    echo ""
    echo "  # Proxy to remote telnet server"
    echo "  $0 192.168.1.100:23"
    echo ""
    echo "  # Proxy with custom WebSocket port"
    echo "  $0 192.168.1.100:23 8080"
    echo ""
    echo "  # Using named options"
    echo "  $0 --telnet-host 192.168.1.100 --telnet-port 23 --ws-port 8080"
    echo ""
    echo "Starting telnet daemon:"
    echo "  busybox telnetd -p 8023 -l /bin/bash"
    exit 0
}

# Parse arguments - support both positional and named options
# Positional: [HOST:PORT] [WS_PORT]
if [[ $# -gt 0 && ! "$1" =~ ^- ]]; then
    # First positional arg: HOST:PORT
    if [[ "$1" =~ : ]]; then
        TELNET_HOST="${1%:*}"
        TELNET_PORT="${1#*:}"
    else
        TELNET_HOST="$1"
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
        -t|--telnet-port)
            TELNET_PORT="$2"
            shift 2
            ;;
        -H|--telnet-host)
            TELNET_HOST="$2"
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

echo "Telnet WebSocket Proxy"
echo "======================"
echo "WebSocket: ws://0.0.0.0:$WS_PORT"
echo "Telnet Server: $TELNET_HOST:$TELNET_PORT"
echo ""

# Try websocat first (lightweight, Rust-based)
if command -v websocat &> /dev/null; then
    echo "Using websocat..."
    exec websocat --binary ws-l:0.0.0.0:$WS_PORT tcp:$TELNET_HOST:$TELNET_PORT
fi

# Try websockify (Python, common with noVNC)
if command -v websockify &> /dev/null; then
    echo "Using websockify..."
    exec websockify 0.0.0.0:$WS_PORT $TELNET_HOST:$TELNET_PORT
fi

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

    const telnet = net.connect($TELNET_PORT, '$TELNET_HOST');

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
                telnet.write(payload);
            } else if (opcode === 0x01) { // Text frame (telnet often uses text)
                telnet.write(payload);
            } else if (opcode === 0x08) { // Close
                socket.end();
                telnet.end();
                return;
            }

            buffer = buffer.slice(offset + payloadLen);
        }
    });

    telnet.on('data', (data) => {
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

    telnet.on('close', () => socket.end());
    telnet.on('error', (e) => { console.error('Telnet error:', e.message); socket.end(); });
    socket.on('close', () => telnet.end());
    socket.on('error', (e) => { console.error('WS error:', e.message); telnet.end(); });
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
