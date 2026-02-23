#!/usr/bin/env python3
"""
Simple HTTP server for yetty web demo.
Serves files with proper CORS and MIME type headers for WebAssembly.
Includes CORS proxy for vfsync.org (JSLinux VFS).

Usage: python serve.py [port] [directory]
"""

import http.server
import socketserver
import sys
import os
import urllib.request
import urllib.error
from functools import partial

class CORSRequestHandler(http.server.SimpleHTTPRequestHandler):
    """HTTP handler with CORS headers, proper WASM MIME type, and vfsync proxy."""

    def end_headers(self):
        # Add CORS headers for cross-origin isolation (required for SharedArrayBuffer)
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')

        # Cache control for development
        self.send_header('Cache-Control', 'no-cache, no-store, must-revalidate')

        super().end_headers()

    def guess_type(self, path):
        """Override to ensure correct MIME types."""
        mimetype = super().guess_type(path)

        # Ensure correct MIME type for WebAssembly
        if path.endswith('.wasm'):
            return 'application/wasm'
        if path.endswith('.js'):
            return 'application/javascript'

        return mimetype

    def do_GET(self):
        """Handle GET requests, including vfsync proxy."""
        # Proxy requests to /vfsync/* to https://vfsync.org/*
        if self.path.startswith('/vfsync/'):
            self.proxy_vfsync()
            return

        # Normal file serving
        super().do_GET()

    def proxy_vfsync(self):
        """Serve local vfsync files first, fallback to vfsync.org proxy."""
        # Extract the path after /vfsync/ and strip query string
        vfsync_path = self.path[8:]  # Remove '/vfsync/'
        if '?' in vfsync_path:
            vfsync_path = vfsync_path.split('?')[0]

        # Try to serve local file first (e.g., ./vfsync/u/os/yetty-alpine/...)
        local_path = os.path.join(os.getcwd(), 'vfsync', vfsync_path)
        if os.path.isfile(local_path):
            try:
                with open(local_path, 'rb') as f:
                    data = f.read()
                self.send_response(200)
                self.send_header('Content-Type', 'application/octet-stream')
                self.send_header('Content-Length', len(data))
                self.end_headers()
                self.wfile.write(data)
                import sys
                sys.stderr.write(f"  [local] {self.path}\n")
                sys.stderr.flush()
                return
            except Exception as e:
                print(f"  [vfsync] Error reading local file {local_path}: {e}")

        # Fallback to proxy to vfsync.org
        import sys
        sys.stderr.write(f"  [proxy] {self.path} -> vfsync.org (local not found: {local_path})\n")
        sys.stderr.flush()
        target_url = f'https://vfsync.org/{vfsync_path}'

        try:
            # Create request with headers that vfsync.org expects
            req = urllib.request.Request(target_url)
            req.add_header('User-Agent', 'Mozilla/5.0')
            req.add_header('Accept', '*/*')
            # Pretend to be from bellard.org
            req.add_header('Origin', 'https://bellard.org')
            req.add_header('Referer', 'https://bellard.org/jslinux/')

            with urllib.request.urlopen(req, timeout=30) as response:
                data = response.read()
                content_type = response.headers.get('Content-Type', 'application/octet-stream')

                self.send_response(200)
                self.send_header('Content-Type', content_type)
                self.send_header('Content-Length', len(data))
                self.end_headers()
                self.wfile.write(data)

        except urllib.error.HTTPError as e:
            self.send_error(e.code, f'Proxy error: {e.reason}')
        except urllib.error.URLError as e:
            self.send_error(502, f'Proxy connection error: {e.reason}')
        except Exception as e:
            self.send_error(500, f'Proxy error: {str(e)}')

    def do_OPTIONS(self):
        """Handle CORS preflight requests."""
        self.send_response(200)
        self.end_headers()

    def log_message(self, format, *args):
        """Custom log format."""
        import sys
        # Handle different log formats - args[0] can be string (request) or int (error code)
        try:
            msg = format % args
            if '/vfsync/' in msg:
                sys.stderr.write(f"  [vfsync] {msg}\n")
            else:
                sys.stderr.write(f"  {msg}\n")
            sys.stderr.flush()
        except:
            sys.stderr.write(f"  {format} {args}\n")
            sys.stderr.flush()


def main():
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 8000
    directory = sys.argv[2] if len(sys.argv) > 2 else '.'

    os.chdir(directory)
    print(f"  DEBUG: CWD = {os.getcwd()}", flush=True)
    vfsync_check = os.path.join(os.getcwd(), 'vfsync', 'u', 'os', 'yetty-alpine', 'head')
    print(f"  DEBUG: yetty-alpine head exists = {os.path.isfile(vfsync_check)}", flush=True)

    handler = partial(CORSRequestHandler, directory=directory)

    with socketserver.TCPServer(("0.0.0.0", port), handler) as httpd:
        print(f"\n  yetty web demo server")
        print(f"  Serving at: http://localhost:{port}/")
        print(f"  Directory:  {os.path.abspath(directory)}")
        print(f"  VFS:        /vfsync/* -> local ./vfsync/ or https://vfsync.org/*")
        print(f"\n  Press Ctrl+C to stop.\n")

        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n  Server stopped.")
            sys.exit(0)


if __name__ == "__main__":
    main()
