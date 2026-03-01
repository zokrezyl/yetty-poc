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
    console.log("[term-bridge] " + msg);
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

        // Output buffering for performance
        this._outputBuffer = '';
        this._flushPending = false;

        // Get ptyId from URL params (set by yetty when creating iframe)
        const params = new URLSearchParams(window.location.search);
        this.ptyId = params.get('ptyId');
        termLog("ptyId from URL: " + this.ptyId);

        // Listen for input from yetty
        window.addEventListener('message', (e) => {
            if (e.data.type === 'term-input' && e.data.ptyId == this.ptyId) {
                var codes = [];
                for (var ci = 0; ci < e.data.data.length; ci++) codes.push(e.data.data.charCodeAt(ci));
                termLog("Received term-input: " + e.data.data.length + " chars, codes=[" + codes.join(',') + "]");
                // Input from yetty keyboard -> send to emulator
                if (this.keyHandler) {
                    this.keyHandler(e.data.data);
                } else {
                    termLog("WARNING: keyHandler not set!");
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
        this._outputBuffer += str;

        if (!this._flushPending) {
            this._flushPending = true;
            setTimeout(() => this._flushOutput(), 0);
        }
    }

    _flushOutput() {
        this._flushPending = false;
        if (this._outputBuffer.length === 0) return;

        var str = this._outputBuffer;
        this._outputBuffer = '';

        // Fix garbled UTF-8: JSLinux's _console_write uses String.fromCharCode on raw bytes
        var fixedStr = str;
        var needsFix = false;
        for (var i = 0; i < str.length; i++) {
            var code = str.charCodeAt(i);
            if (code >= 0x80 && code <= 0xF7) {
                needsFix = true;
                break;
            }
        }

        if (needsFix) {
            var bytes = new Uint8Array(str.length);
            for (var i = 0; i < str.length; i++) {
                bytes[i] = str.charCodeAt(i) & 0xFF;
            }
            try {
                fixedStr = new TextDecoder('utf-8').decode(bytes);
            } catch (e) {
                fixedStr = str;
            }
        }

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
