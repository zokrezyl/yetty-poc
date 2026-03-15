/*
 * test_harness.h - Test framework for ygui-c
 *
 * Provides:
 * - Mock terminal environment (no raw mode)
 * - OSC event injection
 * - Callback tracking
 * - libuv-based event loop testing
 */

#ifndef YGUI_TEST_HARNESS_H
#define YGUI_TEST_HARNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*=============================================================================
 * Test Assertions
 *===========================================================================*/

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("  FAIL: %s (line %d)\n", msg, __LINE__); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        printf("  FAIL: %s: expected %d, got %d (line %d)\n", msg, (int)(b), (int)(a), __LINE__); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_EQ_FLOAT(a, b, eps, msg) do { \
    float _a = (a), _b = (b); \
    if (_a < _b - (eps) || _a > _b + (eps)) { \
        printf("  FAIL: %s: expected %.2f, got %.2f (line %d)\n", msg, _b, _a, __LINE__); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_STR_EQ(a, b, msg) do { \
    const char* _a = (a); const char* _b = (b); \
    if (_a == NULL || _b == NULL || strcmp(_a, _b) != 0) { \
        printf("  FAIL: %s: expected '%s', got '%s' (line %d)\n", msg, _b ? _b : "NULL", _a ? _a : "NULL", __LINE__); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_NOT_NULL(ptr, msg) do { \
    if ((ptr) == NULL) { \
        printf("  FAIL: %s: expected non-NULL (line %d)\n", msg, __LINE__); \
        return 1; \
    } \
} while(0)

#define TEST_ASSERT_NULL(ptr, msg) do { \
    if ((ptr) != NULL) { \
        printf("  FAIL: %s: expected NULL (line %d)\n", msg, __LINE__); \
        return 1; \
    } \
} while(0)

/*=============================================================================
 * Test Runner
 *===========================================================================*/

typedef int (*test_func_t)(void);

typedef struct {
    const char* name;
    test_func_t func;
} test_case_t;

static inline int run_tests(const char* suite_name, test_case_t* tests, int count) {
    printf("\n=== %s ===\n", suite_name);
    int passed = 0, failed = 0;

    for (int i = 0; i < count; i++) {
        printf("Running: %s... ", tests[i].name);
        fflush(stdout);
        int result = tests[i].func();
        if (result == 0) {
            printf("PASS\n");
            passed++;
        } else {
            failed++;
        }
    }

    printf("\nResults: %d passed, %d failed\n", passed, failed);
    return failed;
}

/*=============================================================================
 * Callback Tracking
 *===========================================================================*/

/* Track callback invocations for testing */
typedef struct {
    int click_count;
    int change_count;
    int check_count;
    int key_count;

    /* Last callback values */
    float last_float_value;
    int last_int_value;
    int last_checked;
    uint32_t last_key;
    int last_mods;
    const char* last_widget_id;
} callback_tracker_t;

static callback_tracker_t g_tracker = {0};

static inline void tracker_reset(void) {
    memset(&g_tracker, 0, sizeof(g_tracker));
}

/*=============================================================================
 * OSC Event Generation
 *===========================================================================*/

/* Generate OSC 777777 click event string */
static inline int make_osc_click(char* buf, int bufsize, const char* card_name,
                                  int buttons, int press, int x, int y) {
    return snprintf(buf, bufsize, "\033]777777;%s;%d;%d;%d;%d\033\\",
                    card_name, buttons, press, x, y);
}

/* Generate OSC 777778 move event string */
static inline int make_osc_move(char* buf, int bufsize, const char* card_name,
                                 int buttons, int x, int y) {
    return snprintf(buf, bufsize, "\033]777778;%s;%d;%d;%d\033\\",
                    card_name, buttons, x, y);
}

/* Generate OSC 777779 view change event string */
static inline int make_osc_view_change(char* buf, int bufsize, const char* card_name,
                                        float zoom, float scroll_x, float scroll_y) {
    return snprintf(buf, bufsize, "\033]777779;%s;%.2f;%.0f;%.0f\033\\",
                    card_name, zoom, scroll_x, scroll_y);
}

/* Generate CSI cell size response */
static inline int make_csi_cell_size(char* buf, int bufsize, int height, int width) {
    return snprintf(buf, bufsize, "\033[6;%d;%dt", height, width);
}

#endif /* YGUI_TEST_HARNESS_H */
