/*
 * grid_test.c - Unit tests for spatial grid and hit testing
 */

#include "test_harness.h"
#include "../../../src/yetty/ygui-c/ygui.h"
#include "../../../src/yetty/ygui-c/ygui_internal.h"

/*=============================================================================
 * Grid Basic Tests
 *===========================================================================*/

static int test_grid_init(void) {
    ygui_spatial_grid_t grid;
    ygui_grid_init(&grid, 400, 300, 32.0f);

    TEST_ASSERT(grid.cells != NULL, "cells allocated");
    TEST_ASSERT_EQ(grid.cols, 13, "columns");  /* 400/32 + 1 = 13 */
    TEST_ASSERT_EQ(grid.rows, 10, "rows");     /* 300/32 + 1 = 10 */
    TEST_ASSERT_EQ_FLOAT(grid.cell_size, 32.0f, 0.01, "cell size");

    ygui_grid_destroy(&grid);
    return 0;
}

static int test_grid_insert_query(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    /* Create a button and manually set effective position */
    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Test");
    btn->effective_x = 50;
    btn->effective_y = 50;
    btn->was_rendered = 1;

    /* Insert into grid */
    ygui_grid_insert(&e->grid, btn);

    /* Query inside button */
    ygui_widget_t* hit = ygui_grid_query(&e->grid, 75, 70);
    TEST_ASSERT(hit == btn, "hit inside button");

    /* Query outside button */
    hit = ygui_grid_query(&e->grid, 10, 10);
    TEST_ASSERT_NULL(hit, "no hit outside button");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_query_boundaries(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 100, 100, 50, 30, "Test");
    btn->effective_x = 100;
    btn->effective_y = 100;
    btn->was_rendered = 1;
    ygui_grid_insert(&e->grid, btn);

    /* Exact left edge - should hit */
    ygui_widget_t* hit = ygui_grid_query(&e->grid, 100, 115);
    TEST_ASSERT(hit == btn, "hit at left edge");

    /* Just inside left edge */
    hit = ygui_grid_query(&e->grid, 101, 115);
    TEST_ASSERT(hit == btn, "hit just inside left");

    /* Just outside left edge */
    hit = ygui_grid_query(&e->grid, 99, 115);
    TEST_ASSERT_NULL(hit, "no hit just outside left");

    /* Exact right edge (x=150 is outside, since range is [100, 150)) */
    hit = ygui_grid_query(&e->grid, 149, 115);
    TEST_ASSERT(hit == btn, "hit at right edge -1");

    hit = ygui_grid_query(&e->grid, 150, 115);
    TEST_ASSERT_NULL(hit, "no hit at exact right edge");

    /* Top edge */
    hit = ygui_grid_query(&e->grid, 125, 100);
    TEST_ASSERT(hit == btn, "hit at top edge");

    /* Bottom edge */
    hit = ygui_grid_query(&e->grid, 125, 129);
    TEST_ASSERT(hit == btn, "hit at bottom edge -1");

    hit = ygui_grid_query(&e->grid, 125, 130);
    TEST_ASSERT_NULL(hit, "no hit at exact bottom edge");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_multiple_widgets(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* btn1 = ygui_button(e, "btn1", 10, 10, 80, 30, "One");
    btn1->effective_x = 10; btn1->effective_y = 10;
    btn1->was_rendered = 1;

    ygui_widget_t* btn2 = ygui_button(e, "btn2", 100, 10, 80, 30, "Two");
    btn2->effective_x = 100; btn2->effective_y = 10;
    btn2->was_rendered = 1;

    ygui_widget_t* btn3 = ygui_button(e, "btn3", 10, 50, 80, 30, "Three");
    btn3->effective_x = 10; btn3->effective_y = 50;
    btn3->was_rendered = 1;

    ygui_grid_insert(&e->grid, btn1);
    ygui_grid_insert(&e->grid, btn2);
    ygui_grid_insert(&e->grid, btn3);

    /* Query each button */
    TEST_ASSERT(ygui_grid_query(&e->grid, 50, 25) == btn1, "hit btn1");
    TEST_ASSERT(ygui_grid_query(&e->grid, 140, 25) == btn2, "hit btn2");
    TEST_ASSERT(ygui_grid_query(&e->grid, 50, 65) == btn3, "hit btn3");

    /* Query between buttons */
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 95, 25), "gap between btn1 and btn2");
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 50, 45), "gap between btn1 and btn3");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_overlapping_widgets(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    /* Create two overlapping widgets - later one should be on top */
    ygui_widget_t* btn1 = ygui_button(e, "btn1", 50, 50, 100, 100, "Bottom");
    btn1->effective_x = 50; btn1->effective_y = 50;
    btn1->was_rendered = 1;

    ygui_widget_t* btn2 = ygui_button(e, "btn2", 80, 80, 60, 60, "Top");
    btn2->effective_x = 80; btn2->effective_y = 80;
    btn2->was_rendered = 1;

    ygui_grid_insert(&e->grid, btn1);
    ygui_grid_insert(&e->grid, btn2);

    /* Query in overlap area - should hit top widget (btn2) */
    ygui_widget_t* hit = ygui_grid_query(&e->grid, 100, 100);
    TEST_ASSERT(hit == btn2, "hit top widget in overlap");

    /* Query in btn1 only area */
    hit = ygui_grid_query(&e->grid, 60, 60);
    TEST_ASSERT(hit == btn1, "hit bottom widget in non-overlap");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_clear(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Test");
    btn->effective_x = 50; btn->effective_y = 50;
    btn->was_rendered = 1;
    ygui_grid_insert(&e->grid, btn);

    TEST_ASSERT(ygui_grid_query(&e->grid, 75, 70) == btn, "hit before clear");

    ygui_grid_clear(&e->grid);

    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 75, 70), "no hit after clear");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_invisible_widget(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Test");
    btn->effective_x = 50; btn->effective_y = 50;
    btn->was_rendered = 1;
    ygui_grid_insert(&e->grid, btn);

    /* Hit works when visible */
    TEST_ASSERT(ygui_grid_query(&e->grid, 75, 70) == btn, "hit when visible");

    /* Make invisible */
    ygui_widget_set_visible(btn, 0);
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 75, 70), "no hit when invisible");

    /* Make visible again */
    ygui_widget_set_visible(btn, 1);
    TEST_ASSERT(ygui_grid_query(&e->grid, 75, 70) == btn, "hit when visible again");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_disabled_widget(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Test");
    btn->effective_x = 50; btn->effective_y = 50;
    btn->was_rendered = 1;
    ygui_grid_insert(&e->grid, btn);

    /* Hit works when enabled */
    TEST_ASSERT(ygui_grid_query(&e->grid, 75, 70) == btn, "hit when enabled");

    /* Make disabled */
    ygui_widget_set_enabled(btn, 0);
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 75, 70), "no hit when disabled");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_large_widget(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    /* Widget spanning multiple grid cells */
    ygui_widget_t* btn = ygui_button(e, "btn", 10, 10, 200, 100, "Large");
    btn->effective_x = 10; btn->effective_y = 10;
    btn->was_rendered = 1;
    ygui_grid_insert(&e->grid, btn);

    /* Test hits at various points within the large widget */
    TEST_ASSERT(ygui_grid_query(&e->grid, 15, 15) == btn, "top-left");
    TEST_ASSERT(ygui_grid_query(&e->grid, 100, 55) == btn, "center");
    TEST_ASSERT(ygui_grid_query(&e->grid, 205, 105) == btn, "bottom-right");

    ygui_engine_destroy(e);
    return 0;
}

static int test_grid_out_of_bounds_query(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    /* Query outside grid bounds should return NULL, not crash */
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, -10, 50), "negative x");
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 50, -10), "negative y");
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 500, 50), "x beyond width");
    TEST_ASSERT_NULL(ygui_grid_query(&e->grid, 50, 400), "y beyond height");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Main
 *===========================================================================*/

int main(void) {
    test_case_t tests[] = {
        {"grid_init", test_grid_init},
        {"grid_insert_query", test_grid_insert_query},
        {"grid_query_boundaries", test_grid_query_boundaries},
        {"grid_multiple_widgets", test_grid_multiple_widgets},
        {"grid_overlapping_widgets", test_grid_overlapping_widgets},
        {"grid_clear", test_grid_clear},
        {"grid_invisible_widget", test_grid_invisible_widget},
        {"grid_disabled_widget", test_grid_disabled_widget},
        {"grid_large_widget", test_grid_large_widget},
        {"grid_out_of_bounds_query", test_grid_out_of_bounds_query},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    return run_tests("Spatial Grid Tests", tests, count);
}
