// Yetty WASM API - host functions provided by yetty terminal
#ifndef YETTY_WASM_H
#define YETTY_WASM_H

#include <stdint.h>

// Import declarations for yetty host functions
#define YETTY_IMPORT __attribute__((import_module("yetty")))

// Logging
YETTY_IMPORT __attribute__((import_name("log")))
void yetty_log_raw(const char* ptr, int32_t len);

// Card size
YETTY_IMPORT __attribute__((import_name("get_width")))
int32_t yetty_get_width(void);

YETTY_IMPORT __attribute__((import_name("get_height")))
int32_t yetty_get_height(void);

// Drawing - shapes use SDF rendering
YETTY_IMPORT __attribute__((import_name("draw_rect")))
void yetty_draw_rect(float x, float y, float w, float h, int32_t color);

YETTY_IMPORT __attribute__((import_name("draw_circle")))
void yetty_draw_circle(float cx, float cy, float r, int32_t color);

YETTY_IMPORT __attribute__((import_name("draw_line")))
void yetty_draw_line(float x1, float y1, float x2, float y2, int32_t color, float width);

YETTY_IMPORT __attribute__((import_name("draw_text")))
void yetty_draw_text_raw(const char* ptr, int32_t len, float x, float y, float size, int32_t color);

YETTY_IMPORT __attribute__((import_name("clear")))
void yetty_clear(void);

// Lifecycle
YETTY_IMPORT __attribute__((import_name("request_frame")))
void yetty_request_frame(void);

YETTY_IMPORT __attribute__((import_name("exit")))
void yetty_exit(int32_t code);

// Helper: get string length
static inline int yetty_strlen(const char* s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

// Helper: log a string
static inline void yetty_log(const char* msg) {
    yetty_log_raw(msg, yetty_strlen(msg));
}

// Helper: draw text string
static inline void yetty_draw_text(const char* text, float x, float y, float size, int32_t color) {
    yetty_draw_text_raw(text, yetty_strlen(text), x, y, size, color);
}

#endif // YETTY_WASM_H
