/**
 * File import helper for JSLinux VM
 *
 * Import files into the running JSLinux VM via postMessage.
 * Files are placed in the VM's filesystem and can be executed.
 *
 * Usage:
 *   // Import a single file
 *   await importFileToVM(ptyId, '/home/demo.sh', demoShContent);
 *
 *   // Import multiple files from URLs
 *   await importFilesFromURLs(ptyId, {
 *     '/home/demo.sh': '/demo/demo.sh',
 *     '/home/tool': '/tools/mytool'
 *   });
 */

/**
 * Import a file into the JSLinux VM
 * @param {number} ptyId - PTY ID of the VM iframe
 * @param {string} path - Path in the VM filesystem (e.g., '/home/demo.sh')
 * @param {Uint8Array|string} data - File contents
 * @returns {Promise<void>}
 */
async function importFileToVM(ptyId, path, data) {
    const iframe = document.getElementById('jslinux-pty-' + ptyId);
    if (!iframe || !iframe.contentWindow) {
        throw new Error('VM iframe not found: ' + ptyId);
    }

    // Convert string to Uint8Array if needed
    let bytes;
    if (typeof data === 'string') {
        bytes = new TextEncoder().encode(data);
    } else if (data instanceof Uint8Array) {
        bytes = data;
    } else if (data instanceof ArrayBuffer) {
        bytes = new Uint8Array(data);
    } else {
        throw new Error('Data must be string, Uint8Array, or ArrayBuffer');
    }

    return new Promise((resolve, reject) => {
        const timeout = setTimeout(() => {
            window.removeEventListener('message', handler);
            reject(new Error('Import timeout for ' + path));
        }, 10000);

        function handler(e) {
            if (e.data.type === 'file-imported' && e.data.path === path) {
                clearTimeout(timeout);
                window.removeEventListener('message', handler);
                console.log('Imported:', path, '(' + e.data.size + ' bytes)');
                resolve();
            }
        }
        window.addEventListener('message', handler);

        iframe.contentWindow.postMessage({
            type: 'import-file',
            path: path,
            data: bytes
        }, '*');
    });
}

/**
 * Import multiple files from URLs into the VM
 * @param {number} ptyId - PTY ID
 * @param {Object} fileMap - Map of VM path -> URL
 * @returns {Promise<void>}
 */
async function importFilesFromURLs(ptyId, fileMap) {
    for (const [vmPath, url] of Object.entries(fileMap)) {
        console.log('Fetching:', url, '->', vmPath);
        const response = await fetch(url);
        if (!response.ok) {
            throw new Error('Failed to fetch ' + url + ': ' + response.status);
        }
        const data = new Uint8Array(await response.arrayBuffer());
        await importFileToVM(ptyId, vmPath, data);
    }
}

/**
 * Import demo files after VM boot
 * Call this after the VM shows the login prompt
 * @param {number} ptyId - PTY ID
 */
async function importDemoFiles(ptyId) {
    // Wait for VM to be ready (look for shell prompt)
    console.log('Importing demo files...');

    const files = {
        // Add your demo files here
        // '/home/demo.sh': '/demo/demo.sh',
        // '/usr/local/bin/mytool': '/tools/mytool',
    };

    if (Object.keys(files).length === 0) {
        console.log('No demo files configured');
        return;
    }

    await importFilesFromURLs(ptyId, files);
    console.log('Demo files imported successfully');
}

// Export for module systems
if (typeof module !== 'undefined') {
    module.exports = { importFileToVM, importFilesFromURLs, importDemoFiles };
}
