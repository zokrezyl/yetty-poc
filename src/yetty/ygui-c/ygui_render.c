/*
 * ygui_render.c - Render context implementation
 *
 * This bridges YGui widgets to YDraw buffer.
 * Currently stubbed - needs YDraw C API integration.
 */

#include "ygui_internal.h"

/*=============================================================================
 * Render Context
 *===========================================================================*/

void ygui_render_ctx_init(ygui_render_ctx_t* ctx, ydraw_buffer_t* buffer,
                          const ygui_theme_t* theme) {
    ctx->buffer = buffer;
    ctx->theme = theme;
    ctx->offset_x = 0;
    ctx->offset_y = 0;
    ctx->clip_x = 0;
    ctx->clip_y = 0;
    ctx->clip_w = 0;
    ctx->clip_h = 0;
    ctx->has_clip = 0;
}

/*=============================================================================
 * Drawing Functions
 *
 * These call into YDraw buffer. Currently stubbed until YDraw C API exists.
 * When YDraw C API is ready, these will call:
 *   ydraw_buffer_add_rounded_box(...)
 *   ydraw_buffer_add_text(...)
 *   etc.
 *===========================================================================*/

void ygui_render_box(ygui_render_ctx_t* ctx, float x, float y, float w, float h,
                     uint32_t color, float radius) {
    if (!ctx->buffer) return;

    float ax = x + ctx->offset_x;
    float ay = y + ctx->offset_y;

    /* TODO: Call YDraw C API
     * ydraw_add_rounded_box(ctx->buffer, 0, cx, cy, hw, hh,
     *                       radius, radius, radius, radius,
     *                       color, 0, 0, 0, AUTO_ID);
     */
    (void)ax; (void)ay; (void)w; (void)h; (void)color; (void)radius;
}

void ygui_render_box_outline(ygui_render_ctx_t* ctx, float x, float y, float w, float h,
                             uint32_t color, float radius, float stroke_width) {
    if (!ctx->buffer) return;

    float ax = x + ctx->offset_x;
    float ay = y + ctx->offset_y;

    /* TODO: Call YDraw C API
     * ydraw_add_rounded_box(ctx->buffer, 0, cx, cy, hw, hh,
     *                       radius, radius, radius, radius,
     *                       0, color, stroke_width, 0, AUTO_ID);
     */
    (void)ax; (void)ay; (void)w; (void)h; (void)color; (void)radius; (void)stroke_width;
}

void ygui_render_text(ygui_render_ctx_t* ctx, const char* text, float x, float y,
                      uint32_t color, float font_size) {
    if (!ctx->buffer || !text) return;

    float ax = x + ctx->offset_x;
    float ay = y + ctx->offset_y;

    /* TODO: Call YDraw C API
     * ydraw_add_text(ctx->buffer, ax, ay, text, font_size, color, 0, -1);
     */
    (void)ax; (void)ay; (void)color; (void)font_size;
}

void ygui_render_circle(ygui_render_ctx_t* ctx, float cx, float cy, float r,
                        uint32_t color) {
    if (!ctx->buffer) return;

    float ax = cx + ctx->offset_x;
    float ay = cy + ctx->offset_y;

    /* TODO: Call YDraw C API
     * ydraw_add_circle(ctx->buffer, 0, ax, ay, r, color, 0, 0, 0, AUTO_ID);
     */
    (void)ax; (void)ay; (void)r; (void)color;
}

void ygui_render_triangle(ygui_render_ctx_t* ctx, float x0, float y0,
                          float x1, float y1, float x2, float y2, uint32_t color) {
    if (!ctx->buffer) return;

    float ox = ctx->offset_x;
    float oy = ctx->offset_y;

    /* TODO: Call YDraw C API
     * ydraw_add_triangle(ctx->buffer, 0, x0+ox, y0+oy, x1+ox, y1+oy, x2+ox, y2+oy,
     *                    color, 0, 0, 0, AUTO_ID);
     */
    (void)x0; (void)y0; (void)x1; (void)y1; (void)x2; (void)y2;
    (void)ox; (void)oy; (void)color;
}
