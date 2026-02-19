#!/usr/bin/env python3
"""Lightweight HTTP test server for ybrowser unit tests.

Usage: python3 server.py
Prints "PORT:<port>" to stdout when ready, then serves until killed.
"""

import http.server
import json
import socket
import sys
import urllib.parse

# ─── Test HTML pages ──────────────────────────────────────────────────────────

PAGE_INDEX = """\
<!DOCTYPE html>
<html><head><title>Test Index</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <h1>Test Index Page</h1>
  <p><a href="/page1" style="display:inline-block; padding:10px; font-size:24px;">Go to Page 1</a></p>
  <p><a href="/page2" style="display:inline-block; padding:10px; font-size:24px;">Go to Page 2</a></p>
  <p><a href="/absolute-link" style="display:inline-block; padding:10px; font-size:24px;">Absolute Link</a></p>
</body></html>
"""

PAGE_1 = """\
<!DOCTYPE html>
<html><head><title>Page 1</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <h1>Page One</h1>
  <p>This is page one content.</p>
  <p><a href="/" style="display:inline-block; padding:10px;">Back to Index</a></p>
</body></html>
"""

PAGE_2 = """\
<!DOCTYPE html>
<html><head><title>Page 2</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <h1>Page Two</h1>
  <p>This is page two content.</p>
</body></html>
"""

PAGE_FORM_POST = """\
<!DOCTYPE html>
<html><head><title>POST Form</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <h1>POST Form Test</h1>
  <form action="/submit" method="POST">
    <input type="hidden" name="token" value="abc123">
    <input type="hidden" name="session" value="xyz789">
    <input type="submit" value="Submit POST"
           style="display:inline-block; padding:20px; font-size:24px;">
  </form>
</body></html>
"""

PAGE_FORM_GET = """\
<!DOCTYPE html>
<html><head><title>GET Form</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <h1>GET Form Test</h1>
  <form action="/search" method="GET">
    <input type="hidden" name="q" value="test_query">
    <input type="hidden" name="lang" value="en">
    <button type="submit" style="display:inline-block; padding:20px; font-size:24px;">
      Search
    </button>
  </form>
</body></html>
"""

PAGE_MULTI_FORM = """\
<!DOCTYPE html>
<html><head><title>Multiple Forms</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <form action="/action1" method="POST" id="form1">
    <input type="hidden" name="form_id" value="1">
    <input type="hidden" name="data" value="first">
    <button type="submit" style="display:inline-block; padding:20px; font-size:24px;">
      Submit Form 1
    </button>
  </form>
  <br><br>
  <form action="/action2" method="POST" id="form2">
    <input type="hidden" name="form_id" value="2">
    <input type="hidden" name="data" value="second">
    <button type="submit" style="display:inline-block; padding:20px; font-size:24px;">
      Submit Form 2
    </button>
  </form>
</body></html>
"""

PAGE_MULTI_LINK = """\
<!DOCTYPE html>
<html><head><title>Multiple Links</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <p><a href="/link-a" id="link-a" style="display:block; padding:15px; font-size:24px;">Link A</a></p>
  <p style="margin-top:40px;"><a href="/link-b" id="link-b" style="display:block; padding:15px; font-size:24px;">Link B</a></p>
  <p style="margin-top:40px;"><a href="/link-c" id="link-c" style="display:block; padding:15px; font-size:24px;">Link C</a></p>
</body></html>
"""

PAGE_SPECIAL_CHARS = """\
<!DOCTYPE html>
<html><head><title>Special Characters</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <form action="/submit-special" method="POST">
    <input type="hidden" name="msg" value="hello world">
    <input type="hidden" name="symbols" value="a&amp;b=c">
    <input type="hidden" name="unicode" value="caf&eacute;">
    <button type="submit" style="display:inline-block; padding:20px; font-size:24px;">
      Submit Special
    </button>
  </form>
</body></html>
"""

PAGE_REDIRECT_FORM = """\
<!DOCTYPE html>
<html><head><title>Redirect Form</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <h1>Redirect Form Test</h1>
  <form action="/redirect-submit" method="POST">
    <input type="hidden" name="action" value="redirect_test">
    <button type="submit" style="display:inline-block; padding:20px; font-size:24px;">
      Submit (will redirect)
    </button>
  </form>
</body></html>
"""

PAGE_NESTED = """\
<!DOCTYPE html>
<html><head><title>Nested Structure</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <div style="padding:10px; background:#eee;">
    <div style="padding:10px; background:#ddd;">
      <p><a href="/deep-link" style="display:inline-block; padding:15px; font-size:24px;">Deep Nested Link</a></p>
    </div>
  </div>
  <div style="margin-top:20px;">
    <form action="/nested-submit" method="POST">
      <div style="padding:10px;">
        <input type="hidden" name="level" value="nested">
        <div>
          <button type="submit" style="display:inline-block; padding:20px; font-size:24px;">
            Nested Submit
          </button>
        </div>
      </div>
    </form>
  </div>
</body></html>
"""

PAGE_MIXED = """\
<!DOCTYPE html>
<html><head><title>Mixed Content</title></head>
<body style="margin:0; padding:20px; font-size:16px;">
  <p><a href="/nav-link" style="display:inline-block; padding:10px; font-size:24px;">Navigation Link</a></p>
  <hr>
  <form action="/mixed-submit" method="POST">
    <input type="hidden" name="ctx" value="mixed">
    <button type="submit" style="display:inline-block; padding:20px; font-size:24px;">
      Submit Button
    </button>
  </form>
  <hr>
  <p>Plain text with no interactivity.</p>
</body></html>
"""

PAGE_CSS = """\
body { background: #fff; color: #333; }
a { text-decoration: underline; }
h1 { margin-bottom: 10px; }
"""

# ─── Route table ──────────────────────────────────────────────────────────────

ROUTES = {
    "/":                PAGE_INDEX,
    "/page1":           PAGE_1,
    "/page2":           PAGE_2,
    "/form-post":       PAGE_FORM_POST,
    "/form-get":        PAGE_FORM_GET,
    "/multi-form":      PAGE_MULTI_FORM,
    "/multi-link":      PAGE_MULTI_LINK,
    "/special-chars":   PAGE_SPECIAL_CHARS,
    "/redirect-form":   PAGE_REDIRECT_FORM,
    "/nested":          PAGE_NESTED,
    "/mixed":           PAGE_MIXED,
    "/style.css":       PAGE_CSS,
}


class TestHandler(http.server.BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        pass  # silence request logging

    def do_GET(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path
        query = urllib.parse.parse_qs(parsed.query)

        # Echo endpoint: returns query params as HTML
        if path == "/echo":
            body = "<html><body><h1>Echo</h1><pre>" + json.dumps(query, indent=2) + "</pre></body></html>"
            self._send(200, body)
            return

        # Search endpoint: returns search results as HTML
        if path == "/search":
            q = query.get("q", [""])[0]
            lang = query.get("lang", [""])[0]
            body = (f'<html><body><h1>Search Results</h1>'
                    f'<p>Query: {q}</p><p>Lang: {lang}</p></body></html>')
            self._send(200, body)
            return

        # Redirect target
        if path == "/redirect-target":
            self._send(200, "<html><body><h1>Redirect Target</h1><p>You were redirected here.</p></body></html>")
            return

        # Static routes
        if path in ROUTES:
            content_type = "text/css" if path.endswith(".css") else "text/html"
            self._send(200, ROUTES[path], content_type)
            return

        # Link targets: return simple pages confirming navigation
        if path.startswith("/link-") or path.startswith("/nav-") or path.startswith("/deep-"):
            body = f"<html><body><h1>Arrived at {path}</h1></body></html>"
            self._send(200, body)
            return

        if path == "/absolute-link":
            self._send(200, "<html><body><h1>Absolute Link Target</h1></body></html>")
            return

        self._send(404, f"<html><body><h1>404 Not Found</h1><p>{path}</p></body></html>")

    def do_POST(self):
        content_length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(content_length).decode("utf-8") if content_length else ""
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path

        # Redirect endpoint: POST → 303 → GET /redirect-target
        if path == "/redirect-submit":
            self.send_response(303)
            self.send_header("Location", "/redirect-target")
            self.send_header("Content-Length", "0")
            self.end_headers()
            return

        # Echo POST data back as HTML
        fields = urllib.parse.parse_qs(body) if body else {}
        response = (f'<html><body>'
                    f'<h1>POST Received</h1>'
                    f'<p>Path: {path}</p>'
                    f'<p>Body: {body}</p>'
                    f'<pre>{json.dumps(fields, indent=2)}</pre>'
                    f'</body></html>')
        self._send(200, response)

    def _send(self, code, body, content_type="text/html"):
        data = body.encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", f"{content_type}; charset=utf-8")
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)


def main():
    # Bind to a random available port
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("127.0.0.1", 0))
    port = sock.getsockname()[1]
    sock.close()

    server = http.server.HTTPServer(("127.0.0.1", port), TestHandler)
    print(f"PORT:{port}", flush=True)
    server.serve_forever()


if __name__ == "__main__":
    main()
