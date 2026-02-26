/*
 * Term bridge for yetty integration
 * Replaces the rendering terminal with a postMessage bridge
 *
 * Original term.js Copyright (c) Fabrice Bellard, MIT License
 * Bridge modifications for yetty integration
 */
"use strict";

// Logging helper (disabled for production)
function termLog(msg) {
}

class Term {
    constructor(opts) {
        termLog("Term constructor called");
        this.cols = opts.cols || 80;
        this.rows = opts.rows || 25;
        this.scrollback = opts.scrollback || 0;
        this.fontSize = opts.fontSize || 15;
        this.keyHandler = null;
        this.ptyId = null;

        // Get ptyId from URL params (set by yetty when creating iframe)
        const params = new URLSearchParams(window.location.search);
        this.ptyId = params.get('ptyId');
        termLog("ptyId from URL: " + this.ptyId);

        // Listen for input from yetty
        window.addEventListener('message', (e) => {
            if (e.data.type === 'term-input' && e.data.ptyId == this.ptyId) {
                termLog("Received term-input: " + e.data.data.length + " chars");
                // Input from yetty keyboard -> send to emulator
                if (this.keyHandler) {
                    this.keyHandler(e.data.data);
                }
            }
            if (e.data.type === 'term-resize' && e.data.ptyId == this.ptyId) {
                termLog("Received term-resize: " + e.data.cols + "x" + e.data.rows);
                this.cols = e.data.cols;
                this.rows = e.data.rows;
                // Notify emulator of resize if callback exists
                if (this.resizeHandler) {
                    this.resizeHandler(this.cols, this.rows);
                }
            }
        });

        // Notify parent that we're ready
        termLog("Sending term-ready to parent");
        window.parent.postMessage({
            type: 'term-ready',
            ptyId: this.ptyId,
            cols: this.cols,
            rows: this.rows
        }, '*');
    }

    setKeyHandler(handler) {
        termLog("setKeyHandler called");
        this.keyHandler = handler;
    }

    setResizeHandler(handler) {
        termLog("setResizeHandler called");
        this.resizeHandler = handler;
    }

    open(parentEl) {
        termLog("open called");
        // No DOM rendering - we're just a bridge
        // But mark ourselves as "open"
        this.isOpen = true;
    }

    write(str) {
        // Fix garbled UTF-8: JSLinux's _console_write uses String.fromCharCode on raw bytes,
        // which treats each UTF-8 byte as a separate character. We detect this and reconstruct.
        var fixedStr = str;
        var needsFix = false;
        for (var i = 0; i < str.length; i++) {
            var code = str.charCodeAt(i);
            // UTF-8 continuation bytes (0x80-0xBF) or lead bytes (0xC0-0xF7) appearing as chars
            // indicate garbled UTF-8 from String.fromCharCode
            if (code >= 0x80 && code <= 0xF7) {
                needsFix = true;
                break;
            }
        }

        if (needsFix) {
            // Convert back to bytes and decode as UTF-8
            var bytes = new Uint8Array(str.length);
            for (var i = 0; i < str.length; i++) {
                bytes[i] = str.charCodeAt(i) & 0xFF;
            }
            try {
                fixedStr = new TextDecoder('utf-8').decode(bytes);
            } catch (e) {
                // If decoding fails, use original
                fixedStr = str;
            }
        }

        termLog("write: " + fixedStr.substring(0, 50) + (fixedStr.length > 50 ? "..." : ""));
        // Send output to yetty for rendering
        window.parent.postMessage({
            type: 'term-output',
            ptyId: this.ptyId,
            data: fixedStr
        }, '*');
    }

    writeln(str) {
        this.write(str + '\r\n');
    }

    getSize() {
        return [this.cols, this.rows];
    }

    resizePixel(width, height) {
        termLog("resizePixel: " + width + "x" + height);
        // Calculate new cols/rows based on character size
        // For now, just accept and return true
        return true;
    }

    refresh(ymin, ymax) {
        // No-op - yetty handles rendering
    }

    scroll_disp(n) {
        termLog("scroll_disp: " + n);
        // Send scroll request to yetty
        window.parent.postMessage({
            type: 'term-scroll',
            ptyId: this.ptyId,
            lines: n
        }, '*');
    }

    queue_chars(str) {
        // Async output - just write
        this.write(str);
    }
}

termLog("term-bridge.js loaded");

// Export for module systems
if (typeof module !== 'undefined') {
    module.exports = Term;
}
