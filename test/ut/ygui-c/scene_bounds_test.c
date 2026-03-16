/*
 * Unit tests for ydraw_buffer scene bounds functionality
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* Include the implementation directly to access struct internals */
#include "ydraw-capi.gen.c"

#define ASSERT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        printf("FAIL: %s: expected %g, got %g\n", msg, (double)(b), (double)(a)); \
        return 1; \
    } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        printf("FAIL: %s\n", msg); \
        return 1; \
    } \
} while(0)

static int test_set_scene_bounds_sets_values(void) {
    ydraw_buffer_t* buf = ydraw_buffer_create();
    ASSERT_TRUE(buf != NULL, "buffer create");

    ydraw_buffer_set_scene_bounds(buf, 10.0f, 20.0f, 500.0f, 300.0f);

    ASSERT_EQ(buf->scene_min_x, 10.0f, "scene_min_x");
    ASSERT_EQ(buf->scene_min_y, 20.0f, "scene_min_y");
    ASSERT_EQ(buf->scene_max_x, 500.0f, "scene_max_x");
    ASSERT_EQ(buf->scene_max_y, 300.0f, "scene_max_y");
    ASSERT_EQ(buf->has_scene_bounds, 1, "has_scene_bounds");

    ydraw_buffer_destroy(buf);
    printf("PASS: test_set_scene_bounds_sets_values\n");
    return 0;
}

static int test_clear_resets_has_scene_bounds(void) {
    ydraw_buffer_t* buf = ydraw_buffer_create();
    ASSERT_TRUE(buf != NULL, "buffer create");

    ydraw_buffer_set_scene_bounds(buf, 0.0f, 0.0f, 500.0f, 300.0f);
    ASSERT_EQ(buf->has_scene_bounds, 1, "has_scene_bounds after set");

    ydraw_buffer_clear(buf);
    ASSERT_EQ(buf->has_scene_bounds, 0, "has_scene_bounds after clear");

    ydraw_buffer_destroy(buf);
    printf("PASS: test_clear_resets_has_scene_bounds\n");
    return 0;
}

static int test_scene_bounds_in_serialized_output(void) {
    ydraw_buffer_t* buf = ydraw_buffer_create();
    ASSERT_TRUE(buf != NULL, "buffer create");

    ydraw_buffer_set_scene_bounds(buf, 0.0f, 0.0f, 500.0f, 300.0f);

    const uint8_t* data;
    uint32_t size = ydraw_buffer_serialize(buf, &data);
    ASSERT_TRUE(size >= 25, "serialized size >= 25 for scene metadata");

    /* Scene metadata at end: bgColor(4) + flags(4) + hasSceneBounds(1) + 4 floats(16) = 25 bytes */
    const uint8_t* meta = data + size - 25;

    /* hasSceneBounds at offset 8 */
    ASSERT_EQ(meta[8], 1, "serialized hasSceneBounds");

    /* minX at offset 9 */
    float minX;
    memcpy(&minX, &meta[9], 4);
    ASSERT_EQ(minX, 0.0f, "serialized minX");

    /* minY at offset 13 */
    float minY;
    memcpy(&minY, &meta[13], 4);
    ASSERT_EQ(minY, 0.0f, "serialized minY");

    /* maxX at offset 17 */
    float maxX;
    memcpy(&maxX, &meta[17], 4);
    ASSERT_EQ(maxX, 500.0f, "serialized maxX");

    /* maxY at offset 21 */
    float maxY;
    memcpy(&maxY, &meta[21], 4);
    ASSERT_EQ(maxY, 300.0f, "serialized maxY");

    ydraw_buffer_destroy(buf);
    printf("PASS: test_scene_bounds_in_serialized_output\n");
    return 0;
}

static int test_create_initializes_to_zero(void) {
    ydraw_buffer_t* buf = ydraw_buffer_create();
    ASSERT_TRUE(buf != NULL, "buffer create");

    /* calloc should zero everything */
    ASSERT_EQ(buf->has_scene_bounds, 0, "initial has_scene_bounds");
    ASSERT_EQ(buf->scene_min_x, 0.0f, "initial scene_min_x");
    ASSERT_EQ(buf->scene_min_y, 0.0f, "initial scene_min_y");
    ASSERT_EQ(buf->scene_max_x, 0.0f, "initial scene_max_x");
    ASSERT_EQ(buf->scene_max_y, 0.0f, "initial scene_max_y");

    ydraw_buffer_destroy(buf);
    printf("PASS: test_create_initializes_to_zero\n");
    return 0;
}

int main(void) {
    int failures = 0;

    failures += test_create_initializes_to_zero();
    failures += test_set_scene_bounds_sets_values();
    failures += test_clear_resets_has_scene_bounds();
    failures += test_scene_bounds_in_serialized_output();

    printf("\n%d test(s) failed\n", failures);
    return failures;
}
