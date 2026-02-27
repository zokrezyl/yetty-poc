#!/usr/bin/env python3
"""
Create a bundled filesystem for GitHub Pages deployment.

This creates a compressed archive of the vfsync filesystem that can be:
1. Uploaded to GitHub Releases
2. Served via jsDelivr CDN
3. Extracted client-side for JSLinux

Usage:
    python create-fs-bundle.py [filesystem] [output_dir]
"""

import os
import sys
import json
import gzip
import struct
import urllib.request
import urllib.error
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib

VFSYNC_BASE = "https://vfsync.org"

def fetch_url(url):
    """Fetch URL with proper headers."""
    req = urllib.request.Request(url)
    req.add_header('User-Agent', 'Mozilla/5.0')
    req.add_header('Accept', '*/*')
    req.add_header('Origin', 'https://bellard.org')
    req.add_header('Referer', 'https://bellard.org/jslinux/')

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

def create_bundle(filesystem="alpine-x86_64", output_dir="."):
    """Create a compressed filesystem bundle."""
    output = Path(output_dir)
    output.mkdir(parents=True, exist_ok=True)

    print(f"Creating bundle for {filesystem}")

    # Fetch head
    head_url = f"{VFSYNC_BASE}/u/os/{filesystem}/head"
    print(f"Fetching {head_url}...")
    head_data = fetch_url(head_url)
    head = parse_head(head_data)

    next_file_id = int(head.get('NextFileID', 0), 16) if head.get('NextFileID', '').startswith('0') or not head.get('NextFileID', '').isdigit() else int(head.get('NextFileID', 0))

    # Handle hex NextFileID
    try:
        next_file_id = int(head.get('NextFileID', '0'), 16)
    except ValueError:
        next_file_id = int(head.get('NextFileID', '0'))

    print(f"  FSFileCount: {head.get('FSFileCount', 'unknown')}")
    print(f"  NextFileID: {next_file_id} (0x{next_file_id:x})")
    print(f"  FSSize: {int(head.get('FSSize', 0)) / 1024 / 1024:.1f} MB")

    # Create bundle file - simple format:
    # [4 bytes: magic "VFS1"]
    # [4 bytes: file count]
    # [head data length][head data]
    # For each file: [4 bytes: file_id][4 bytes: data_length][data]

    bundle_path = output / f"{filesystem}.vfs.gz"
    files_data = {}

    def fetch_file(file_id):
        hex_id = format(file_id, 'x')
        url = f"{VFSYNC_BASE}/u/os/{filesystem}/fileid/{hex_id}"
        try:
            data = fetch_url(url)
            return (file_id, data)
        except urllib.error.HTTPError as e:
            if e.code == 404:
                return (file_id, None)
            raise
        except Exception as e:
            print(f"\nError fetching {hex_id}: {e}")
            return (file_id, None)

    print(f"\nDownloading {next_file_id - 1} files (this will take a while)...")

    completed = 0
    with ThreadPoolExecutor(max_workers=20) as executor:
        futures = {executor.submit(fetch_file, i): i for i in range(1, next_file_id)}

        for future in as_completed(futures):
            file_id, data = future.result()
            if data is not None:
                files_data[file_id] = data
            completed += 1
            if completed % 100 == 0:
                print(f"\rProgress: {completed}/{next_file_id-1} files, {len(files_data)} downloaded", end="", flush=True)

    print(f"\n\nDownloaded {len(files_data)} files")

    # Write bundle
    print(f"Writing bundle to {bundle_path}...")

    with gzip.open(bundle_path, 'wb', compresslevel=6) as f:
        # Magic
        f.write(b'VFS1')
        # File count
        f.write(struct.pack('<I', len(files_data) + 1))  # +1 for head
        # Head
        f.write(struct.pack('<I', 0))  # file_id 0 = head
        f.write(struct.pack('<I', len(head_data)))
        f.write(head_data)
        # Files
        for file_id in sorted(files_data.keys()):
            data = files_data[file_id]
            f.write(struct.pack('<I', file_id))
            f.write(struct.pack('<I', len(data)))
            f.write(data)

    bundle_size = bundle_path.stat().st_size
    print(f"Bundle created: {bundle_path}")
    print(f"Bundle size: {bundle_size / 1024 / 1024:.1f} MB")

    # Create JavaScript loader
    loader_js = f'''
// VFS Bundle Loader for {filesystem}
// Load from: {filesystem}.vfs.gz

async function loadVFSBundle(url) {{
    console.log('Loading VFS bundle from', url);
    const response = await fetch(url);
    const compressed = await response.arrayBuffer();

    // Decompress using DecompressionStream (modern browsers)
    const ds = new DecompressionStream('gzip');
    const decompressed = await new Response(
        new Blob([compressed]).stream().pipeThrough(ds)
    ).arrayBuffer();

    const view = new DataView(decompressed);
    let offset = 0;

    // Check magic
    const magic = String.fromCharCode(
        view.getUint8(offset), view.getUint8(offset+1),
        view.getUint8(offset+2), view.getUint8(offset+3)
    );
    if (magic !== 'VFS1') throw new Error('Invalid VFS bundle');
    offset += 4;

    // File count
    const fileCount = view.getUint32(offset, true);
    offset += 4;

    console.log('VFS bundle has', fileCount, 'files');

    // Build file map
    const files = {{}};
    for (let i = 0; i < fileCount; i++) {{
        const fileId = view.getUint32(offset, true);
        offset += 4;
        const dataLen = view.getUint32(offset, true);
        offset += 4;
        files[fileId] = new Uint8Array(decompressed, offset, dataLen);
        offset += dataLen;
    }}

    return files;
}}

// VFS filesystem handler for JSLinux
class BundledVFS {{
    constructor(files) {{
        this.files = files;
        this.head = files[0] ? new TextDecoder().decode(files[0]) : '';
    }}

    getHead() {{
        return this.head;
    }}

    getFile(fileId) {{
        return this.files[fileId] || null;
    }}
}}
'''
    loader_path = output / f"{filesystem}-loader.js"
    loader_path.write_text(loader_js)
    print(f"Loader: {loader_path}")

    return True

def main():
    filesystem = sys.argv[1] if len(sys.argv) > 1 else "alpine-x86_64"
    output_dir = sys.argv[2] if len(sys.argv) > 2 else "."

    print("=" * 60)
    print("VFS Bundle Creator for GitHub Pages")
    print("=" * 60)
    print(f"\nFilesystem: {filesystem}")
    print(f"Output: {output_dir}")
    print("\nWARNING: This will download ~1.6GB of data!")

    response = input("\nContinue? [y/N] ")
    if response.lower() != 'y':
        print("Aborted.")
        return

    create_bundle(filesystem, output_dir)

if __name__ == "__main__":
    main()
