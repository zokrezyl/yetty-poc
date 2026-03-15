// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

#include "ydraw-capi.gen.h"
#include <stdlib.h>
#include <string.h>

/*=============================================================================
 * Buffer structure - simple auto-growing array
 *===========================================================================*/

#define YDRAW_INITIAL_CAPACITY 4096  /* Initial capacity in words (floats) */
#define YDRAW_INITIAL_TEXT_CAPACITY 64

/* Internal text span with owned string */
typedef struct {
    ydraw_text_span_t span;  /* Public data (text points to str_copy) */
    char* str_copy;          /* Owned copy of text string */
} ydraw_text_span_internal_t;

struct ydraw_buffer {
    /* Primitive data */
    float* data;
    uint32_t capacity;
    uint32_t used;
    uint32_t prim_count;

    /* Text spans */
    ydraw_text_span_internal_t* text_spans;
    uint32_t text_capacity;
    uint32_t text_count;

    /* Serialization buffer */
    uint8_t* serial_data;
    uint32_t serial_capacity;
    uint32_t serial_size;

    /* Scene bounds */
    float scene_min_x, scene_min_y;
    float scene_max_x, scene_max_y;
    int has_scene_bounds;
};

/* Write uint32 to float buffer (bitcast) */
static inline void write_u32(float* buf, uint32_t off, uint32_t val) {
    memcpy(&buf[off], &val, sizeof(uint32_t));
}

/* Ensure buffer can hold additional words. Grows by doubling if needed.
 * Returns 1 on success, 0 on OOM. */
static int ensure_capacity(ydraw_buffer_t* buf, uint32_t additional_words) {
    uint32_t required = buf->used + additional_words;
    if (required <= buf->capacity) {
        return 1;  /* Already have space */
    }

    /* Double until we have enough */
    uint32_t new_cap = buf->capacity;
    while (new_cap < required) {
        new_cap *= 2;
    }

    float* new_data = (float*)realloc(buf->data, new_cap * sizeof(float));
    if (!new_data) {
        return 0;  /* OOM */
    }

    buf->data = new_data;
    buf->capacity = new_cap;
    return 1;
}

/*=============================================================================
 * Buffer lifecycle
 *===========================================================================*/

ydraw_buffer_t* ydraw_buffer_create(void) {
    ydraw_buffer_t* buf = (ydraw_buffer_t*)calloc(1, sizeof(ydraw_buffer_t));
    if (!buf) return NULL;

    buf->data = (float*)malloc(YDRAW_INITIAL_CAPACITY * sizeof(float));
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    buf->capacity = YDRAW_INITIAL_CAPACITY;
    buf->used = 0;
    buf->prim_count = 0;

    buf->text_spans = (ydraw_text_span_internal_t*)malloc(
        YDRAW_INITIAL_TEXT_CAPACITY * sizeof(ydraw_text_span_internal_t));
    if (!buf->text_spans) {
        free(buf->data);
        free(buf);
        return NULL;
    }
    buf->text_capacity = YDRAW_INITIAL_TEXT_CAPACITY;
    buf->text_count = 0;

    return buf;
}

void ydraw_buffer_destroy(ydraw_buffer_t* buf) {
    if (!buf) return;
    /* Free text string copies */
    for (uint32_t i = 0; i < buf->text_count; i++) {
        free(buf->text_spans[i].str_copy);
    }
    free(buf->text_spans);
    free(buf->data);
    free(buf);
}

void ydraw_buffer_clear(ydraw_buffer_t* buf) {
    if (!buf) return;
    buf->used = 0;
    buf->prim_count = 0;
    /* Free text string copies */
    for (uint32_t i = 0; i < buf->text_count; i++) {
        free(buf->text_spans[i].str_copy);
    }
    buf->text_count = 0;
    buf->has_scene_bounds = 0;
    /* Keep allocated memory for reuse */
}

void ydraw_buffer_set_scene_bounds(ydraw_buffer_t* buf, float minX, float minY, float maxX, float maxY) {
    if (!buf) return;
    buf->scene_min_x = minX;
    buf->scene_min_y = minY;
    buf->scene_max_x = maxX;
    buf->scene_max_y = maxY;
    buf->has_scene_bounds = 1;
}

uint32_t ydraw_buffer_prim_count(const ydraw_buffer_t* buf) {
    return buf ? buf->prim_count : 0;
}

uint32_t ydraw_buffer_word_count(const ydraw_buffer_t* buf) {
    return buf ? buf->used : 0;
}

uint32_t ydraw_buffer_byte_size(const ydraw_buffer_t* buf) {
    return buf ? buf->used * sizeof(float) : 0;
}

const float* ydraw_buffer_data(const ydraw_buffer_t* buf) {
    return buf ? buf->data : NULL;
}

/*=============================================================================
 * Serialization - compatible with C++ YDrawBuffer::serialize()
 *===========================================================================*/

#define YDRAW_SERIALIZE_MAGIC   0x59445246  /* "YDRF" */
#define YDRAW_SERIALIZE_VERSION 1

uint32_t ydraw_buffer_serialize(ydraw_buffer_t* buf, const uint8_t** out_data) {
    if (!buf || !out_data) {
        if (out_data) *out_data = NULL;
        return 0;
    }

    /* Calculate required size */
    size_t size = 8;  /* magic + version */
    size += 8;  /* primCount + totalWords */
    size += buf->used * sizeof(float);  /* prim data */
    size += 4;  /* fontCount (always 0 for now) */
    size += 4;  /* spanCount */
    for (uint32_t i = 0; i < buf->text_count; i++) {
        size += 4 * 7;  /* x, y, fontSize, color, layer, fontId, rotation */
        size += 4;  /* textLen */
        size += strlen(buf->text_spans[i].span.text);
    }
    size += 4 + 4 + 1 + 16;  /* scene metadata */

    /* Allocate/reallocate serialization buffer */
    if (buf->serial_capacity < size) {
        uint8_t* new_buf = (uint8_t*)realloc(buf->serial_data, size);
        if (!new_buf) {
            *out_data = NULL;
            return 0;
        }
        buf->serial_data = new_buf;
        buf->serial_capacity = (uint32_t)size;
    }

    uint8_t* p = buf->serial_data;

    /* Magic & version */
    memcpy(p, &(uint32_t){YDRAW_SERIALIZE_MAGIC}, 4); p += 4;
    memcpy(p, &(uint32_t){YDRAW_SERIALIZE_VERSION}, 4); p += 4;

    /* Primitives */
    memcpy(p, &buf->prim_count, 4); p += 4;
    memcpy(p, &buf->used, 4); p += 4;
    if (buf->used > 0) {
        memcpy(p, buf->data, buf->used * sizeof(float));
        p += buf->used * sizeof(float);
    }

    /* Fonts (none for now) */
    memcpy(p, &(uint32_t){0}, 4); p += 4;

    /* Text spans */
    memcpy(p, &buf->text_count, 4); p += 4;
    for (uint32_t i = 0; i < buf->text_count; i++) {
        ydraw_text_span_t* ts = &buf->text_spans[i].span;
        memcpy(p, &ts->x, 4); p += 4;
        memcpy(p, &ts->y, 4); p += 4;
        memcpy(p, &ts->fontSize, 4); p += 4;
        memcpy(p, &ts->color, 4); p += 4;
        memcpy(p, &ts->layer, 4); p += 4;
        int32_t fontId = ts->fontId;
        memcpy(p, &fontId, 4); p += 4;
        memcpy(p, &ts->rotation, 4); p += 4;
        uint32_t textLen = (uint32_t)strlen(ts->text);
        memcpy(p, &textLen, 4); p += 4;
        memcpy(p, ts->text, textLen); p += textLen;
    }

    /* Scene metadata */
    memcpy(p, &(uint32_t){0}, 4); p += 4;  /* bgColor */
    memcpy(p, &(uint32_t){0}, 4); p += 4;  /* flags */
    *p++ = buf->has_scene_bounds ? 1 : 0;  /* hasSceneBounds */
    memcpy(p, &buf->scene_min_x, 4); p += 4;  /* minX */
    memcpy(p, &buf->scene_min_y, 4); p += 4;  /* minY */
    memcpy(p, &buf->scene_max_x, 4); p += 4;  /* maxX */
    memcpy(p, &buf->scene_max_y, 4); p += 4;  /* maxY */

    buf->serial_size = (uint32_t)(p - buf->serial_data);
    *out_data = buf->serial_data;
    return buf->serial_size;
}

/*=============================================================================
 * Text span functions
 *===========================================================================*/

static int ensure_text_capacity(ydraw_buffer_t* buf) {
    if (buf->text_count < buf->text_capacity) return 1;

    uint32_t new_cap = buf->text_capacity * 2;
    ydraw_text_span_internal_t* new_spans = (ydraw_text_span_internal_t*)realloc(
        buf->text_spans, new_cap * sizeof(ydraw_text_span_internal_t));
    if (!new_spans) return 0;

    buf->text_spans = new_spans;
    buf->text_capacity = new_cap;
    return 1;
}

int32_t ydraw_add_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                       float fontSize, uint32_t color, uint32_t layer, int fontId) {
    if (!buf || !text) return -1;
    if (!ensure_text_capacity(buf)) return -1;

    size_t len = strlen(text);
    char* str_copy = (char*)malloc(len + 1);
    if (!str_copy) return -1;
    memcpy(str_copy, text, len + 1);

    ydraw_text_span_internal_t* ts = &buf->text_spans[buf->text_count];
    ts->str_copy = str_copy;
    ts->span.x = x;
    ts->span.y = y;
    ts->span.text = str_copy;
    ts->span.fontSize = fontSize;
    ts->span.color = color;
    ts->span.layer = layer;
    ts->span.fontId = fontId;
    ts->span.rotation = 0.0f;

    return (int32_t)buf->text_count++;
}

int32_t ydraw_add_rotated_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                               float fontSize, uint32_t color, float rotation, int fontId) {
    if (!buf || !text) return -1;
    if (!ensure_text_capacity(buf)) return -1;

    size_t len = strlen(text);
    char* str_copy = (char*)malloc(len + 1);
    if (!str_copy) return -1;
    memcpy(str_copy, text, len + 1);

    ydraw_text_span_internal_t* ts = &buf->text_spans[buf->text_count];
    ts->str_copy = str_copy;
    ts->span.x = x;
    ts->span.y = y;
    ts->span.text = str_copy;
    ts->span.fontSize = fontSize;
    ts->span.color = color;
    ts->span.layer = 0;
    ts->span.fontId = fontId;
    ts->span.rotation = rotation;

    return (int32_t)buf->text_count++;
}

uint32_t ydraw_buffer_text_span_count(const ydraw_buffer_t* buf) {
    return buf ? buf->text_count : 0;
}

const ydraw_text_span_t* ydraw_buffer_get_text_span(const ydraw_buffer_t* buf, uint32_t index) {
    if (!buf || index >= buf->text_count) return NULL;
    return &buf->text_spans[index].span;
}

/*=============================================================================
 * Add primitive functions
 *===========================================================================*/

int32_t ydraw_add_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 0u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 1u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = hw;
    p[5] = hh;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_segment(ydraw_buffer_t* buf, uint32_t layer, float x0, float y0, float x1, float y1, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 2u);
    write_u32(p, 1, layer);
    p[2] = x0;
    p[3] = y0;
    p[4] = x1;
    p[5] = y1;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_triangle(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float vx, float vy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 3u);
    write_u32(p, 1, layer);
    p[2] = ax;
    p[3] = ay;
    p[4] = bx;
    p[5] = by;
    p[6] = vx;
    p[7] = vy;
    write_u32(p, 8, fillColor);
    write_u32(p, 9, strokeColor);
    p[10] = strokeWidth;
    p[11] = round_;

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_bezier2(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 4u);
    write_u32(p, 1, layer);
    p[2] = ax;
    p[3] = ay;
    p[4] = bx;
    p[5] = by;
    p[6] = cx;
    p[7] = cy;
    write_u32(p, 8, fillColor);
    write_u32(p, 9, strokeColor);
    p[10] = strokeWidth;
    p[11] = round_;

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_bezier3(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 14)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 5u);
    write_u32(p, 1, layer);
    p[2] = ax;
    p[3] = ay;
    p[4] = bx;
    p[5] = by;
    p[6] = cx;
    p[7] = cy;
    p[8] = dx;
    p[9] = dy;
    write_u32(p, 10, fillColor);
    write_u32(p, 11, strokeColor);
    p[12] = strokeWidth;
    p[13] = round_;

    buf->used += 14;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_ellipse(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float rx, float ry, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 6u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = rx;
    p[5] = ry;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_arc(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 7u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = sc_x;
    p[5] = sc_y;
    p[6] = ra;
    p[7] = rb;
    write_u32(p, 8, fillColor);
    write_u32(p, 9, strokeColor);
    p[10] = strokeWidth;
    p[11] = round_;

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_rounded_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float r0, float r1, float r2, float r3, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 14)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 8u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = hw;
    p[5] = hh;
    p[6] = r0;
    p[7] = r1;
    p[8] = r2;
    p[9] = r3;
    write_u32(p, 10, fillColor);
    write_u32(p, 11, strokeColor);
    p[12] = strokeWidth;
    p[13] = round_;

    buf->used += 14;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_rhombus(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float bx, float by, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 9u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = bx;
    p[5] = by;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_pentagon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 10u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_hexagon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 11u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_star(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float n, float m, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 12u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    p[5] = n;
    p[6] = m;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_pie(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 13u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = sc_x;
    p[5] = sc_y;
    p[6] = r;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_ring(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float nx, float ny, float r, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 14u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = nx;
    p[5] = ny;
    p[6] = r;
    p[7] = th;
    write_u32(p, 8, fillColor);
    write_u32(p, 9, strokeColor);
    p[10] = strokeWidth;
    p[11] = round_;

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_heart(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 15u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = scale;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 16u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = bx;
    p[5] = by;
    p[6] = r;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_rounded_x(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float w, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 17u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = w;
    p[5] = r;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_capsule(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 18u);
    write_u32(p, 1, layer);
    p[2] = ax;
    p[3] = ay;
    p[4] = bx;
    p[5] = by;
    p[6] = r;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_moon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float d, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 19u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = d;
    p[5] = ra;
    p[6] = rb;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_egg(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 20u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = ra;
    p[5] = rb;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_chamfer_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float chamfer, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 21u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = hw;
    p[5] = hh;
    p[6] = chamfer;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_oriented_box(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 22u);
    write_u32(p, 1, layer);
    p[2] = ax;
    p[3] = ay;
    p[4] = bx;
    p[5] = by;
    p[6] = th;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_trapezoid(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r1, float r2, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 23u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r1;
    p[5] = r2;
    p[6] = he;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_parallelogram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wi, float he, float sk, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 24u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = wi;
    p[5] = he;
    p[6] = sk;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_equilateral_triangle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 25u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_isosceles_triangle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float qx, float qy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 26u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = qx;
    p[5] = qy;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_uneven_capsule(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r1, float r2, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 27u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r1;
    p[5] = r2;
    p[6] = h;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_octogon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 28u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_hexagram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 29u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_pentagram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 30u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_cut_disk(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 31u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    p[5] = h;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_horseshoe(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, float wx, float wy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 13)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 32u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = sc_x;
    p[5] = sc_y;
    p[6] = r;
    p[7] = wx;
    p[8] = wy;
    write_u32(p, 9, fillColor);
    write_u32(p, 10, strokeColor);
    p[11] = strokeWidth;
    p[12] = round_;

    buf->used += 13;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_vesica(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float w, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 33u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = w;
    p[5] = h;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_oriented_vesica(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float w, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 34u);
    write_u32(p, 1, layer);
    p[2] = ax;
    p[3] = ay;
    p[4] = bx;
    p[5] = by;
    p[6] = w;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_rounded_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 35u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = h;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_parabola(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float k, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 36u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = k;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_blobby_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 37u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = he;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_tunnel(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 38u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = wh_x;
    p[5] = wh_y;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_stairs(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, float n, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 39u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = wh_x;
    p[5] = wh_y;
    p[6] = n;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_quadratic_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 40u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = scale;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_hyperbola(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float k, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 41u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = k;
    p[5] = he;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_cool_s(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 9)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 42u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = scale;
    write_u32(p, 5, fillColor);
    write_u32(p, 6, strokeColor);
    p[7] = strokeWidth;
    p[8] = round_;

    buf->used += 9;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_circle_wave(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float tb, float ra, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 43u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = tb;
    p[5] = ra;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_color_wheel(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float outerR, float innerR, float hue, float sat, float val, float indicatorSize, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 14)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 44u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = outerR;
    p[5] = innerR;
    p[6] = hue;
    p[7] = sat;
    p[8] = val;
    p[9] = indicatorSize;
    write_u32(p, 10, fillColor);
    write_u32(p, 11, strokeColor);
    p[12] = strokeWidth;
    p[13] = round_;

    buf->used += 14;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_text_glyph(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, uint32_t glyphIndex, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 64u);
    write_u32(p, 1, layer);
    p[2] = x;
    p[3] = y;
    p[4] = scaleX;
    p[5] = scaleY;
    write_u32(p, 6, glyphIndex);
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_rotated_glyph(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, float angle, uint32_t glyphIndex, float cosAngle, float sinAngle, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 14)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 65u);
    write_u32(p, 1, layer);
    p[2] = x;
    p[3] = y;
    p[4] = scaleX;
    p[5] = scaleY;
    p[6] = angle;
    write_u32(p, 7, glyphIndex);
    p[8] = cosAngle;
    p[9] = sinAngle;
    write_u32(p, 10, fillColor);
    write_u32(p, 11, strokeColor);
    p[12] = strokeWidth;
    p[13] = round_;

    buf->used += 14;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_sphere_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 100u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = r;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_box_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float bx, float by, float bz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 101u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = bx;
    p[6] = by;
    p[7] = bz;
    write_u32(p, 8, fillColor);
    write_u32(p, 9, strokeColor);
    p[10] = strokeWidth;
    p[11] = round_;

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_torus_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float majorR, float minorR, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 103u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = majorR;
    p[6] = minorR;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_cylinder_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 105u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = r;
    p[6] = h;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_vertical_capsule_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 11)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 108u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = h;
    p[6] = r;
    write_u32(p, 7, fillColor);
    write_u32(p, 8, strokeColor);
    p[9] = strokeWidth;
    p[10] = round_;

    buf->used += 11;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_capped_cone_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, float r1, float r2, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 110u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = h;
    p[6] = r1;
    p[7] = r2;
    write_u32(p, 8, fillColor);
    write_u32(p, 9, strokeColor);
    p[10] = strokeWidth;
    p[11] = round_;

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_octahedron_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float s, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 115u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = s;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_pyramid_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 116u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = h;
    write_u32(p, 6, fillColor);
    write_u32(p, 7, strokeColor);
    p[8] = strokeWidth;
    p[9] = round_;

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_ellipsoid_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float rx, float ry, float rz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 117u);
    write_u32(p, 1, layer);
    p[2] = px;
    p[3] = py;
    p[4] = pz;
    p[5] = rx;
    p[6] = ry;
    p[7] = rz;
    write_u32(p, 8, fillColor);
    write_u32(p, 9, strokeColor);
    p[10] = strokeWidth;
    p[11] = round_;

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_plot(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float w, float h, uint32_t dataCount, float minVal, float maxVal, uint32_t flags, uint32_t lineColor, uint32_t bgColor) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 12)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 128u);
    write_u32(p, 1, layer);
    p[2] = x;
    p[3] = y;
    p[4] = w;
    p[5] = h;
    write_u32(p, 6, dataCount);
    p[7] = minVal;
    p[8] = maxVal;
    write_u32(p, 9, flags);
    write_u32(p, 10, lineColor);
    write_u32(p, 11, bgColor);

    buf->used += 12;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_image(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float w, float h, uint32_t atlasX, uint32_t atlasY, uint32_t texW, uint32_t texH) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 10)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 129u);
    write_u32(p, 1, layer);
    p[2] = x;
    p[3] = y;
    p[4] = w;
    p[5] = h;
    write_u32(p, 6, atlasX);
    write_u32(p, 7, atlasY);
    write_u32(p, 8, texW);
    write_u32(p, 9, texH);

    buf->used += 10;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_polygon(ydraw_buffer_t* buf, uint32_t layer, uint32_t vertexCount, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 7)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 130u);
    write_u32(p, 1, layer);
    write_u32(p, 2, vertexCount);
    write_u32(p, 3, fillColor);
    write_u32(p, 4, strokeColor);
    p[5] = strokeWidth;
    p[6] = round_;

    buf->used += 7;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_polygon_group(ydraw_buffer_t* buf, uint32_t layer, uint32_t vertexCount, uint32_t contourCount, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 8)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 131u);
    write_u32(p, 1, layer);
    write_u32(p, 2, vertexCount);
    write_u32(p, 3, contourCount);
    write_u32(p, 4, fillColor);
    write_u32(p, 5, strokeColor);
    p[6] = strokeWidth;
    p[7] = round_;

    buf->used += 8;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_linear_gradient_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float gx1, float gy1, float gx2, float gy2, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 15)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 132u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = hw;
    p[5] = hh;
    p[6] = gx1;
    p[7] = gy1;
    p[8] = gx2;
    p[9] = gy2;
    write_u32(p, 10, color1);
    write_u32(p, 11, color2);
    write_u32(p, 12, strokeColor);
    p[13] = strokeWidth;
    p[14] = round_;

    buf->used += 15;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_linear_gradient_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float gx1, float gy1, float gx2, float gy2, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 14)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 133u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    p[5] = gx1;
    p[6] = gy1;
    p[7] = gx2;
    p[8] = gy2;
    write_u32(p, 9, color1);
    write_u32(p, 10, color2);
    write_u32(p, 11, strokeColor);
    p[12] = strokeWidth;
    p[13] = round_;

    buf->used += 14;
    return (int32_t)buf->prim_count++;
}

int32_t ydraw_add_radial_gradient_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float gcx, float gcy, float gr, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf) return -1;
    if (!ensure_capacity(buf, 13)) return -1;

    float* p = buf->data + buf->used;
    write_u32(p, 0, 134u);
    write_u32(p, 1, layer);
    p[2] = cx;
    p[3] = cy;
    p[4] = r;
    p[5] = gcx;
    p[6] = gcy;
    p[7] = gr;
    write_u32(p, 8, color1);
    write_u32(p, 9, color2);
    write_u32(p, 10, strokeColor);
    p[11] = strokeWidth;
    p[12] = round_;

    buf->used += 13;
    return (int32_t)buf->prim_count++;
}
