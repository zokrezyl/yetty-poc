// Simple hello world for yetty wasm card
#include "yetty.h"

void _start(void) {
    yetty_log("Hello from WebAssembly!");

    // Draw a red rectangle
    yetty_draw_rect(10.0f, 10.0f, 200.0f, 100.0f, 0xFF0000FF);  // RGBA red

    // Draw a blue circle
    yetty_draw_circle(300.0f, 60.0f, 40.0f, 0x0000FFFF);  // RGBA blue

    // Draw a green line
    yetty_draw_line(10.0f, 150.0f, 350.0f, 150.0f, 0x00FF00FF, 3.0f);

    // Draw some text
    yetty_draw_text("WASM works!", 50.0f, 50.0f, 24.0f, 0xFFFFFFFF);

    yetty_exit(0);
}
