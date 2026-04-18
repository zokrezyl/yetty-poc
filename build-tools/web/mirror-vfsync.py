#!/usr/bin/env python3
"""
Mirror a vfsync filesystem for static hosting (GitHub Pages).

URL format: /files/[16-digit hex id]
Example: https://vfsync.org/u/os/alpine-x86_64/files/0000000000000fd5

Usage:
    python mirror-vfsync.py [output_dir] [filesystem]

Example:
    python mirror-vfsync.py ./vfsync alpine-x86_64
"""

import os
import sys
import urllib.request
import urllib.error
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed

VFSYNC_BASE = "https://vfsync.org"

def fetch_url(url):
    """Fetch URL with proper headers."""
    req = urllib.request.Request(url)
    req.add_header('User-Agent', 'Mozilla/5.0')
    req.add_header('Accept', '*/*')
    with urllib.request.urlopen(req, timeout=60) as response:
        return response.read()

def parse_head(content):
    """Parse the vfsync head file."""
    result = {}
    for line in content.decode('utf-8').strip().split('\n'):
        if ':' in line:
            key, value = line.split(':', 1)
            result[key.strip()] = value.strip()
    return result

def mirror_filesystem(output_dir, filesystem="alpine-x86_64"):
    """Mirror a vfsync filesystem to local directory."""
    output = Path(output_dir) / "u" / "os" / filesystem
    output.mkdir(parents=True, exist_ok=True)
    files_dir = output / "files"
    files_dir.mkdir(exist_ok=True)

    print(f"Mirroring {filesystem} to {output}")

    # Fetch head
    head_url = f"{VFSYNC_BASE}/u/os/{filesystem}/head"
    print(f"Fetching {head_url}...")
    head_data = fetch_url(head_url)
    (output / "head").write_bytes(head_data)
    head = parse_head(head_data)

    next_file_id = int(head.get('NextFileID', '0'))
    fs_size = int(head.get('FSSize', '0'))
    file_count = int(head.get('FSFileCount', '0'))

    print(f"  Files: {file_count}")
    print(f"  Size: {fs_size / 1024 / 1024:.1f} MB")
    print(f"  NextFileID: {next_file_id} (0x{next_file_id:x})")

    print(f"\nThis will download ~{fs_size / 1024 / 1024:.0f} MB")
    response = input("Continue? [y/N] ")
    if response.lower() != 'y':
        print("Aborted.")
        return False

    def fetch_file(file_id):
        hex_id = f"{file_id:016x}"
        url = f"{VFSYNC_BASE}/u/os/{filesystem}/files/{hex_id}"
        out_file = files_dir / hex_id

        if out_file.exists():
            return (file_id, True, "cached", out_file.stat().st_size)

        try:
            data = fetch_url(url)
            out_file.write_bytes(data)
            return (file_id, True, "ok", len(data))
        except urllib.error.HTTPError as e:
            if e.code == 404:
                return (file_id, True, "404", 0)
            return (file_id, False, str(e), 0)
        except Exception as e:
            return (file_id, False, str(e), 0)

    completed = 0
    downloaded = 0
    total_bytes = 0
    errors = 0

    with ThreadPoolExecutor(max_workers=20) as executor:
        futures = {executor.submit(fetch_file, i): i for i in range(1, next_file_id)}

        for future in as_completed(futures):
            file_id, success, status, size = future.result()
            completed += 1
            if success and status == "ok":
                downloaded += 1
                total_bytes += size
            elif not success:
                errors += 1
                print(f"\nError on {file_id:x}: {status}")

            if completed % 100 == 0:
                mb = total_bytes / 1024 / 1024
                print(f"\rProgress: {completed}/{next_file_id-1} | Downloaded: {downloaded} ({mb:.1f} MB) | Errors: {errors}", end="", flush=True)

    print(f"\n\nDone!")
    print(f"  Files: {downloaded}")
    print(f"  Size: {total_bytes / 1024 / 1024:.1f} MB")
    print(f"  Location: {output}")

    # Create config for local serving
    config = f"""/* Local vfsync config for GitHub Pages */
{{
    version: 1,
    machine: "pc",
    memory_size: 256,
    kernel: "kernel-x86_64.bin",
    cmdline: "loglevel=3 console=hvc0 root=root rootfstype=9p rootflags=trans=virtio ro TZ=${{TZ}}",
    fs0: {{ file: "vfsync/u/os/{filesystem}" }},
    eth0: {{ driver: "user" }},
}}
"""
    config_path = Path(output_dir).parent / f"{filesystem}-local.cfg"
    config_path.write_text(config)
    print(f"  Config: {config_path}")

    return True

if __name__ == "__main__":
    output_dir = sys.argv[1] if len(sys.argv) > 1 else "./vfsync"
    filesystem = sys.argv[2] if len(sys.argv) > 2 else "alpine-x86_64"
    mirror_filesystem(output_dir, filesystem)
