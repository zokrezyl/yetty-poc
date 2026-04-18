/*
 * osc_parsing_test.c - Tests for OSC/CSI input parsing
 *
 * Tests the parsing of:
 * - OSC 777777 (mouse click)
 * - OSC 777778 (mouse move)
 * - OSC 777779 (view change)
 * - CSI 6;h;w t (cell size response)
 */

#include "test_harness.h"
#include <string.h>
#include <stdio.h>

/*
 * Replicate the parsing functions from ygui_engine.c for testing
 */

static int parse_card_mouse_osc(const char* buf, int len,
                                 int* osc_code, char* card_name, int name_max,
                                 int* buttons, int* press,
                                 float* x, float* y, int* consumed) {
    if (len < 10) return 0;
    if (buf[0] != '\033' || buf[1] != ']') return 0;

    int i = 2;
    int code = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        code = code * 10 + (buf[i] - '0');
        i++;
    }
    if (code != 777777 && code != 777778) return 0;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    int name_start = i;
    while (i < len && buf[i] != ';') i++;
    if (i >= len) return 0;
    int name_len = i - name_start;
    if (name_len >= name_max) name_len = name_max - 1;
    memcpy(card_name, buf + name_start, name_len);
    card_name[name_len] = '\0';
    i++;

    int btn = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        btn = btn * 10 + (buf[i] - '0');
        i++;
    }
    *buttons = btn;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    if (code == 777777) {
        int p = 0;
        while (i < len && buf[i] >= '0' && buf[i] <= '9') {
            p = p * 10 + (buf[i] - '0');
            i++;
        }
        *press = p;
        if (i >= len || buf[i] != ';') return 0;
        i++;
    } else {
        *press = -1;
    }

    int ix = 0;
    int neg = 0;
    if (i < len && buf[i] == '-') { neg = 1; i++; }
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        ix = ix * 10 + (buf[i] - '0');
        i++;
    }
    *x = neg ? -(float)ix : (float)ix;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    int iy = 0;
    neg = 0;
    if (i < len && buf[i] == '-') { neg = 1; i++; }
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        iy = iy * 10 + (buf[i] - '0');
        i++;
    }
    *y = neg ? -(float)iy : (float)iy;

    if (i + 1 >= len || buf[i] != '\033' || buf[i+1] != '\\') return 0;

    *osc_code = code;
    *consumed = i + 2;
    return 1;
}

static int parse_cell_size_csi(const char* buf, int len,
                                int* cell_height, int* cell_width, int* consumed) {
    if (len < 8) return 0;
    if (buf[0] != '\033' || buf[1] != '[') return 0;

    int i = 2;
    int n1 = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        n1 = n1 * 10 + (buf[i] - '0');
        i++;
    }
    if (n1 != 6) return 0;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    int h = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        h = h * 10 + (buf[i] - '0');
        i++;
    }
    if (i >= len || buf[i] != ';') return 0;
    i++;

    int w = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        w = w * 10 + (buf[i] - '0');
        i++;
    }
    if (i >= len || buf[i] != 't') return 0;
    i++;

    *cell_height = h;
    *cell_width = w;
    *consumed = i;
    return 1;
}

static int parse_view_change_osc(const char* buf, int len,
                                  char* card_name, int name_max,
                                  float* zoom, float* scroll_x, float* scroll_y,
                                  int* consumed) {
    if (len < 12) return 0;
    if (buf[0] != '\033' || buf[1] != ']') return 0;

    int i = 2;
    int code = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        code = code * 10 + (buf[i] - '0');
        i++;
    }
    if (code != 777779) return 0;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    int name_start = i;
    while (i < len && buf[i] != ';') i++;
    if (i >= len) return 0;
    int name_len = i - name_start;
    if (name_len >= name_max) name_len = name_max - 1;
    memcpy(card_name, buf + name_start, name_len);
    card_name[name_len] = '\0';
    i++;

    /* Parse zoom (float) */
    float z = 0.0f;
    int neg = 0;
    if (i < len && buf[i] == '-') { neg = 1; i++; }
    while (i < len && (buf[i] >= '0' && buf[i] <= '9')) {
        z = z * 10.0f + (float)(buf[i] - '0');
        i++;
    }
    if (i < len && buf[i] == '.') {
        i++;
        float frac = 0.1f;
        while (i < len && buf[i] >= '0' && buf[i] <= '9') {
            z += (float)(buf[i] - '0') * frac;
            frac *= 0.1f;
            i++;
        }
    }
    *zoom = neg ? -z : z;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    /* Parse scroll_x */
    float sx = 0.0f;
    neg = 0;
    if (i < len && buf[i] == '-') { neg = 1; i++; }
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        sx = sx * 10.0f + (float)(buf[i] - '0');
        i++;
    }
    if (i < len && buf[i] == '.') {
        i++;
        float frac = 0.1f;
        while (i < len && buf[i] >= '0' && buf[i] <= '9') {
            sx += (float)(buf[i] - '0') * frac;
            frac *= 0.1f;
            i++;
        }
    }
    *scroll_x = neg ? -sx : sx;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    /* Parse scroll_y */
    float sy = 0.0f;
    neg = 0;
    if (i < len && buf[i] == '-') { neg = 1; i++; }
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        sy = sy * 10.0f + (float)(buf[i] - '0');
        i++;
    }
    if (i < len && buf[i] == '.') {
        i++;
        float frac = 0.1f;
        while (i < len && buf[i] >= '0' && buf[i] <= '9') {
            sy += (float)(buf[i] - '0') * frac;
            frac *= 0.1f;
            i++;
        }
    }
    *scroll_y = neg ? -sy : sy;

    if (i + 1 >= len || buf[i] != '\033' || buf[i+1] != '\\') return 0;

    *consumed = i + 2;
    return 1;
}

/*=============================================================================
 * OSC 777777 (Click) Tests
 *===========================================================================*/

static int test_parse_click_press(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777777;mycard;1;1;150;80\033\\");

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, len, &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ(code, 777777, "OSC code");
    TEST_ASSERT_STR_EQ(card_name, "mycard", "card name");
    TEST_ASSERT_EQ(buttons, 1, "buttons");
    TEST_ASSERT_EQ(press, 1, "press=1 (down)");
    TEST_ASSERT_EQ_FLOAT(x, 150.0f, 0.01f, "x coord");
    TEST_ASSERT_EQ_FLOAT(y, 80.0f, 0.01f, "y coord");
    TEST_ASSERT_EQ(consumed, len, "consumed all bytes");

    return 0;
}

static int test_parse_click_release(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777777;card1;1;0;200;100\033\\");

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, len, &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ(press, 0, "press=0 (release)");

    return 0;
}

static int test_parse_click_negative_coords(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777777;card;1;1;-50;-30\033\\");

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, len, &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ_FLOAT(x, -50.0f, 0.01f, "negative x");
    TEST_ASSERT_EQ_FLOAT(y, -30.0f, 0.01f, "negative y");

    return 0;
}

/*=============================================================================
 * OSC 777778 (Move) Tests
 *===========================================================================*/

static int test_parse_move(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777778;dashboard;0;250;125\033\\");

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, len, &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ(code, 777778, "OSC code for move");
    TEST_ASSERT_STR_EQ(card_name, "dashboard", "card name");
    TEST_ASSERT_EQ(buttons, 0, "no buttons");
    TEST_ASSERT_EQ(press, -1, "press N/A for move");
    TEST_ASSERT_EQ_FLOAT(x, 250.0f, 0.01f, "x coord");
    TEST_ASSERT_EQ_FLOAT(y, 125.0f, 0.01f, "y coord");

    return 0;
}

static int test_parse_move_with_button(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777778;card;1;300;200\033\\");

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, len, &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ(buttons, 1, "left button held during drag");

    return 0;
}

/*=============================================================================
 * OSC 777779 (View Change) Tests
 *===========================================================================*/

static int test_parse_view_change(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777779;mycard;1.5;100;200\033\\");

    char card_name[64];
    float zoom, scroll_x, scroll_y;
    int consumed;

    int ok = parse_view_change_osc(buf, len, card_name, sizeof(card_name),
                                    &zoom, &scroll_x, &scroll_y, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_STR_EQ(card_name, "mycard", "card name");
    TEST_ASSERT_EQ_FLOAT(zoom, 1.5f, 0.01f, "zoom");
    TEST_ASSERT_EQ_FLOAT(scroll_x, 100.0f, 0.01f, "scroll_x");
    TEST_ASSERT_EQ_FLOAT(scroll_y, 200.0f, 0.01f, "scroll_y");

    return 0;
}

static int test_parse_view_change_decimal(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777779;card;2.25;50.5;75.75\033\\");

    char card_name[64];
    float zoom, scroll_x, scroll_y;
    int consumed;

    int ok = parse_view_change_osc(buf, len, card_name, sizeof(card_name),
                                    &zoom, &scroll_x, &scroll_y, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ_FLOAT(zoom, 2.25f, 0.01f, "zoom with decimal");
    TEST_ASSERT_EQ_FLOAT(scroll_x, 50.5f, 0.1f, "scroll_x with decimal");
    TEST_ASSERT_EQ_FLOAT(scroll_y, 75.75f, 0.1f, "scroll_y with decimal");

    return 0;
}

/*=============================================================================
 * CSI Cell Size Response Tests
 *===========================================================================*/

static int test_parse_cell_size(void) {
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "\033[6;16;8t");

    int height, width, consumed;
    int ok = parse_cell_size_csi(buf, len, &height, &width, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ(height, 16, "cell height");
    TEST_ASSERT_EQ(width, 8, "cell width");
    TEST_ASSERT_EQ(consumed, len, "consumed all bytes");

    return 0;
}

static int test_parse_cell_size_large(void) {
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "\033[6;24;12t");

    int height, width, consumed;
    int ok = parse_cell_size_csi(buf, len, &height, &width, &consumed);

    TEST_ASSERT(ok, "parse success");
    TEST_ASSERT_EQ(height, 24, "cell height");
    TEST_ASSERT_EQ(width, 12, "cell width");

    return 0;
}

/*=============================================================================
 * Invalid Input Tests
 *===========================================================================*/

static int test_parse_invalid_osc_code(void) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]123456;card;1;1;100;50\033\\");

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, len, &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(!ok, "should fail for invalid OSC code");
    return 0;
}

static int test_parse_truncated_input(void) {
    char buf[] = "\033]777777;card;1;1";  /* Missing coords and ST */

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, (int)strlen(buf), &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(!ok, "should fail for truncated input");
    return 0;
}

static int test_parse_missing_st(void) {
    char buf[] = "\033]777777;card;1;1;100;50";  /* Missing ESC \ */

    int code, buttons, press, consumed;
    char card_name[64];
    float x, y;

    int ok = parse_card_mouse_osc(buf, (int)strlen(buf), &code, card_name, sizeof(card_name),
                                   &buttons, &press, &x, &y, &consumed);

    TEST_ASSERT(!ok, "should fail for missing ST");
    return 0;
}

/*=============================================================================
 * Main
 *===========================================================================*/

int main(void) {
    test_case_t tests[] = {
        /* Click events */
        {"parse_click_press", test_parse_click_press},
        {"parse_click_release", test_parse_click_release},
        {"parse_click_negative_coords", test_parse_click_negative_coords},

        /* Move events */
        {"parse_move", test_parse_move},
        {"parse_move_with_button", test_parse_move_with_button},

        /* View change */
        {"parse_view_change", test_parse_view_change},
        {"parse_view_change_decimal", test_parse_view_change_decimal},

        /* Cell size */
        {"parse_cell_size", test_parse_cell_size},
        {"parse_cell_size_large", test_parse_cell_size_large},

        /* Invalid input */
        {"parse_invalid_osc_code", test_parse_invalid_osc_code},
        {"parse_truncated_input", test_parse_truncated_input},
        {"parse_missing_st", test_parse_missing_st},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    return run_tests("OSC Parsing Tests", tests, count);
}
