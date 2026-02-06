"""Base64 encoding/decoding for yetty OSC payloads.

Uses standard base64 encoding which is widely available in most languages
and command-line tools (base64 command, Python base64 module, etc.).
"""

import base64


def encode(data: bytes) -> str:
    """Encode bytes to base64 string."""
    return base64.b64encode(data).decode('ascii')


def decode(encoded: str) -> bytes:
    """Decode base64 string back to bytes."""
    return base64.b64decode(encoded)


def encode_string(text: str) -> str:
    """Encode a UTF-8 string to base64."""
    return encode(text.encode('utf-8'))


def decode_string(encoded: str) -> str:
    """Decode base64 to UTF-8 string."""
    return decode(encoded).decode('utf-8')
