/*
 * widget_test.c - Unit tests for ygui-c widgets
 *
 * Tests widget creation, properties, and basic functionality.
 * Does NOT test rendering or events (see integration_test.c for that).
 */

#include "test_harness.h"
#include "../../../src/yetty/ygui-c/ygui.h"
#include "../../../src/yetty/ygui-c/ygui_internal.h"

/*=============================================================================
 * Button Widget Tests
 *===========================================================================*/

static int test_button_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    TEST_ASSERT_NOT_NULL(e, "engine create");

    ygui_widget_t* btn = ygui_button(e, "btn1", 10, 20, 100, 40, "Click Me");
    TEST_ASSERT_NOT_NULL(btn, "button create");
    TEST_ASSERT_EQ(btn->type, YGUI_WIDGET_BUTTON, "widget type");
    TEST_ASSERT_STR_EQ(btn->id, "btn1", "widget id");
    TEST_ASSERT_EQ_FLOAT(btn->x, 10, 0.01, "x position");
    TEST_ASSERT_EQ_FLOAT(btn->y, 20, 0.01, "y position");
    TEST_ASSERT_EQ_FLOAT(btn->w, 100, 0.01, "width");
    TEST_ASSERT_EQ_FLOAT(btn->h, 40, 0.01, "height");
    TEST_ASSERT_STR_EQ(btn->data.button.label, "Click Me", "label");

    ygui_engine_destroy(e);
    return 0;
}

static int test_button_set_label(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* btn = ygui_button(e, "btn", 0, 0, 100, 40, "Original");

    ygui_button_set_label(btn, "Updated");
    TEST_ASSERT_STR_EQ(ygui_button_get_label(btn), "Updated", "label updated");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Label Widget Tests
 *===========================================================================*/

static int test_label_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* lbl = ygui_label(e, "lbl1", 50, 100, "Hello World");
    TEST_ASSERT_NOT_NULL(lbl, "label create");
    TEST_ASSERT_EQ(lbl->type, YGUI_WIDGET_LABEL, "widget type");
    TEST_ASSERT_STR_EQ(lbl->data.label.text, "Hello World", "text");

    ygui_engine_destroy(e);
    return 0;
}

static int test_label_set_text(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* lbl = ygui_label(e, "lbl", 0, 0, "Original");

    ygui_label_set_text(lbl, "New Text");
    TEST_ASSERT_STR_EQ(ygui_label_get_text(lbl), "New Text", "text updated");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Slider Widget Tests
 *===========================================================================*/

static int test_slider_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* sld = ygui_slider(e, "slider1", 10, 50, 200, 30, 0, 100, 50);
    TEST_ASSERT_NOT_NULL(sld, "slider create");
    TEST_ASSERT_EQ(sld->type, YGUI_WIDGET_SLIDER, "widget type");
    TEST_ASSERT_EQ_FLOAT(sld->data.slider.min_val, 0, 0.01, "min value");
    TEST_ASSERT_EQ_FLOAT(sld->data.slider.max_val, 100, 0.01, "max value");
    TEST_ASSERT_EQ_FLOAT(sld->data.slider.value, 50, 0.01, "initial value");

    ygui_engine_destroy(e);
    return 0;
}

static int test_slider_set_value(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* sld = ygui_slider(e, "sld", 0, 0, 200, 30, 0, 100, 50);

    ygui_slider_set_value(sld, 75);
    TEST_ASSERT_EQ_FLOAT(ygui_slider_get_value(sld), 75, 0.01, "value updated");

    /* Test clamping */
    ygui_slider_set_value(sld, 150);
    TEST_ASSERT_EQ_FLOAT(ygui_slider_get_value(sld), 100, 0.01, "value clamped high");

    ygui_slider_set_value(sld, -10);
    TEST_ASSERT_EQ_FLOAT(ygui_slider_get_value(sld), 0, 0.01, "value clamped low");

    ygui_engine_destroy(e);
    return 0;
}

static int test_slider_set_range(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* sld = ygui_slider(e, "sld", 0, 0, 200, 30, 0, 100, 50);

    ygui_slider_set_range(sld, 10, 20);
    TEST_ASSERT_EQ_FLOAT(sld->data.slider.min_val, 10, 0.01, "min updated");
    TEST_ASSERT_EQ_FLOAT(sld->data.slider.max_val, 20, 0.01, "max updated");
    /* Value should be clamped to new range */
    TEST_ASSERT_EQ_FLOAT(ygui_slider_get_value(sld), 20, 0.01, "value clamped to new range");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Checkbox Widget Tests
 *===========================================================================*/

static int test_checkbox_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* chk = ygui_checkbox(e, "chk1", 10, 50, 200, 30, "Enable", 0);
    TEST_ASSERT_NOT_NULL(chk, "checkbox create");
    TEST_ASSERT_EQ(chk->type, YGUI_WIDGET_CHECKBOX, "widget type");
    TEST_ASSERT_STR_EQ(chk->data.checkbox.label, "Enable", "label");
    TEST_ASSERT_EQ(chk->data.checkbox.checked, 0, "initial unchecked");

    ygui_widget_t* chk2 = ygui_checkbox(e, "chk2", 10, 90, 200, 30, "Feature", 1);
    TEST_ASSERT_EQ(chk2->data.checkbox.checked, 1, "initial checked");

    ygui_engine_destroy(e);
    return 0;
}

static int test_checkbox_set_checked(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* chk = ygui_checkbox(e, "chk", 0, 0, 200, 30, "Test", 0);

    ygui_checkbox_set_checked(chk, 1);
    TEST_ASSERT_EQ(ygui_checkbox_get_checked(chk), 1, "checked set to true");

    ygui_checkbox_set_checked(chk, 0);
    TEST_ASSERT_EQ(ygui_checkbox_get_checked(chk), 0, "checked set to false");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Progress Widget Tests
 *===========================================================================*/

static int test_progress_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* prg = ygui_progress(e, "prog1", 10, 50, 300, 20, 0.5);
    TEST_ASSERT_NOT_NULL(prg, "progress create");
    TEST_ASSERT_EQ(prg->type, YGUI_WIDGET_PROGRESS, "widget type");
    TEST_ASSERT_EQ_FLOAT(prg->data.progress.value, 0.5, 0.01, "initial value");

    ygui_engine_destroy(e);
    return 0;
}

static int test_progress_set_value(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* prg = ygui_progress(e, "prog", 0, 0, 300, 20, 0);

    ygui_progress_set_value(prg, 0.75);
    TEST_ASSERT_EQ_FLOAT(ygui_progress_get_value(prg), 0.75, 0.01, "value updated");

    /* Test clamping */
    ygui_progress_set_value(prg, 1.5);
    TEST_ASSERT_EQ_FLOAT(ygui_progress_get_value(prg), 1.0, 0.01, "value clamped high");

    ygui_progress_set_value(prg, -0.5);
    TEST_ASSERT_EQ_FLOAT(ygui_progress_get_value(prg), 0.0, 0.01, "value clamped low");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Panel Widget Tests
 *===========================================================================*/

static int test_panel_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* pnl = ygui_panel(e, "panel1", 10, 10, 200, 150);
    TEST_ASSERT_NOT_NULL(pnl, "panel create");
    TEST_ASSERT_EQ(pnl->type, YGUI_WIDGET_PANEL, "widget type");

    ygui_engine_destroy(e);
    return 0;
}

static int test_panel_content_size(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* pnl = ygui_panel(e, "panel", 0, 0, 200, 150);

    ygui_panel_set_content_size(pnl, 200, 500);
    TEST_ASSERT_EQ_FLOAT(pnl->data.panel.content_w, 200, 0.01, "content width");
    TEST_ASSERT_EQ_FLOAT(pnl->data.panel.content_h, 500, 0.01, "content height");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Dropdown Widget Tests
 *===========================================================================*/

static int test_dropdown_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    const char* options[] = {"Option 1", "Option 2", "Option 3"};
    ygui_widget_t* dd = ygui_dropdown(e, "dropdown1", 10, 50, 150, 30, options, 3);
    TEST_ASSERT_NOT_NULL(dd, "dropdown create");
    TEST_ASSERT_EQ(dd->type, YGUI_WIDGET_DROPDOWN, "widget type");
    TEST_ASSERT_EQ(dd->data.dropdown.option_count, 3, "option count");
    TEST_ASSERT_EQ(dd->data.dropdown.selected, 0, "initial selected");
    TEST_ASSERT_STR_EQ(dd->data.dropdown.options[0], "Option 1", "option 0");
    TEST_ASSERT_STR_EQ(dd->data.dropdown.options[1], "Option 2", "option 1");
    TEST_ASSERT_STR_EQ(dd->data.dropdown.options[2], "Option 3", "option 2");

    ygui_engine_destroy(e);
    return 0;
}

static int test_dropdown_set_selected(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    const char* options[] = {"A", "B", "C"};
    ygui_widget_t* dd = ygui_dropdown(e, "dd", 0, 0, 150, 30, options, 3);

    ygui_dropdown_set_selected(dd, 2);
    TEST_ASSERT_EQ(ygui_dropdown_get_selected(dd), 2, "selected updated");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * TextInput Widget Tests
 *===========================================================================*/

static int test_textinput_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* txt = ygui_textinput(e, "input1", 10, 50, 200, 30, "Enter text...");
    TEST_ASSERT_NOT_NULL(txt, "textinput create");
    TEST_ASSERT_EQ(txt->type, YGUI_WIDGET_TEXTINPUT, "widget type");
    TEST_ASSERT_STR_EQ(txt->data.textinput.placeholder, "Enter text...", "placeholder");
    TEST_ASSERT_STR_EQ(txt->data.textinput.text, "", "initial text empty");

    ygui_engine_destroy(e);
    return 0;
}

static int test_textinput_set_text(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* txt = ygui_textinput(e, "input", 0, 0, 200, 30, "");

    ygui_textinput_set_text(txt, "Hello World");
    TEST_ASSERT_STR_EQ(ygui_textinput_get_text(txt), "Hello World", "text updated");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * ColorPicker Widget Tests
 *===========================================================================*/

static int test_colorpicker_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* cp = ygui_colorpicker(e, "color1", 10, 50, 200, 150);
    TEST_ASSERT_NOT_NULL(cp, "colorpicker create");
    TEST_ASSERT_EQ(cp->type, YGUI_WIDGET_COLORPICKER, "widget type");

    ygui_engine_destroy(e);
    return 0;
}

static int test_colorpicker_set_get_color(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* cp = ygui_colorpicker(e, "color", 0, 0, 200, 150);

    ygui_colorpicker_set_color(cp, 1.0, 0.0, 0.0, 1.0);  /* Red */
    float r, g, b, a;
    ygui_colorpicker_get_color(cp, &r, &g, &b, &a);
    TEST_ASSERT_EQ_FLOAT(r, 1.0, 0.05, "red component");
    TEST_ASSERT_EQ_FLOAT(g, 0.0, 0.05, "green component");
    TEST_ASSERT_EQ_FLOAT(b, 0.0, 0.05, "blue component");
    TEST_ASSERT_EQ_FLOAT(a, 1.0, 0.01, "alpha component");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * HBox/VBox Widget Tests
 *===========================================================================*/

static int test_hbox_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* hbox = ygui_hbox(e, "hbox1", 10, 10, 300, 50);
    TEST_ASSERT_NOT_NULL(hbox, "hbox create");
    TEST_ASSERT_EQ(hbox->type, YGUI_WIDGET_HBOX, "widget type");

    ygui_engine_destroy(e);
    return 0;
}

static int test_vbox_create(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    ygui_widget_t* vbox = ygui_vbox(e, "vbox1", 10, 10, 200, 300);
    TEST_ASSERT_NOT_NULL(vbox, "vbox create");
    TEST_ASSERT_EQ(vbox->type, YGUI_WIDGET_VBOX, "widget type");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Widget Visibility/Enabled Tests
 *===========================================================================*/

static int test_widget_visibility(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* btn = ygui_button(e, "btn", 0, 0, 100, 40, "Test");

    TEST_ASSERT(ygui_widget_is_visible(btn), "initially visible");

    ygui_widget_set_visible(btn, 0);
    TEST_ASSERT(!ygui_widget_is_visible(btn), "hidden");

    ygui_widget_set_visible(btn, 1);
    TEST_ASSERT(ygui_widget_is_visible(btn), "visible again");

    ygui_engine_destroy(e);
    return 0;
}

static int test_widget_enabled(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* btn = ygui_button(e, "btn", 0, 0, 100, 40, "Test");

    TEST_ASSERT(ygui_widget_is_enabled(btn), "initially enabled");

    ygui_widget_set_enabled(btn, 0);
    TEST_ASSERT(!ygui_widget_is_enabled(btn), "disabled");

    ygui_widget_set_enabled(btn, 1);
    TEST_ASSERT(ygui_widget_is_enabled(btn), "enabled again");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Widget Hierarchy Tests
 *===========================================================================*/

static int test_widget_add_child(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* panel = ygui_panel(e, "panel", 0, 0, 200, 200);
    ygui_widget_t* btn = ygui_button(e, "btn", 10, 10, 80, 30, "Child");

    ygui_widget_add_child(panel, btn);

    TEST_ASSERT(ygui_widget_parent(btn) == panel, "parent set");
    TEST_ASSERT(ygui_widget_first_child(panel) == btn, "first child");

    ygui_engine_destroy(e);
    return 0;
}

static int test_widget_multiple_children(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_widget_t* panel = ygui_panel(e, "panel", 0, 0, 200, 200);
    ygui_widget_t* btn1 = ygui_button(e, "btn1", 10, 10, 80, 30, "First");
    ygui_widget_t* btn2 = ygui_button(e, "btn2", 10, 50, 80, 30, "Second");
    ygui_widget_t* btn3 = ygui_button(e, "btn3", 10, 90, 80, 30, "Third");

    ygui_widget_add_child(panel, btn1);
    ygui_widget_add_child(panel, btn2);
    ygui_widget_add_child(panel, btn3);

    TEST_ASSERT(ygui_widget_first_child(panel) == btn1, "first child");
    TEST_ASSERT(ygui_widget_next_sibling(btn1) == btn2, "second child");
    TEST_ASSERT(ygui_widget_next_sibling(btn2) == btn3, "third child");
    TEST_ASSERT_NULL(ygui_widget_next_sibling(btn3), "no more siblings");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Engine Tests
 *===========================================================================*/

static int test_engine_find(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);
    ygui_button(e, "btn1", 10, 10, 100, 40, "One");
    ygui_button(e, "btn2", 10, 60, 100, 40, "Two");
    ygui_label(e, "lbl1", 10, 110, "Label");

    TEST_ASSERT_NOT_NULL(ygui_engine_find(e, "btn1"), "find btn1");
    TEST_ASSERT_NOT_NULL(ygui_engine_find(e, "btn2"), "find btn2");
    TEST_ASSERT_NOT_NULL(ygui_engine_find(e, "lbl1"), "find lbl1");
    TEST_ASSERT_NULL(ygui_engine_find(e, "nonexistent"), "find nonexistent");

    ygui_engine_destroy(e);
    return 0;
}

static int test_engine_dirty_flag(void) {
    ygui_engine_t* e = ygui_engine_create("test", 400, 300);

    /* New engine should be dirty */
    TEST_ASSERT(ygui_engine_is_dirty(e), "initially dirty");

    e->dirty = 0;
    TEST_ASSERT(!ygui_engine_is_dirty(e), "not dirty after clear");

    /* Creating widget should mark dirty */
    ygui_button(e, "btn", 0, 0, 100, 40, "Test");
    TEST_ASSERT(ygui_engine_is_dirty(e), "dirty after widget create");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Main
 *===========================================================================*/

int main(void) {
    test_case_t tests[] = {
        /* Button */
        {"button_create", test_button_create},
        {"button_set_label", test_button_set_label},

        /* Label */
        {"label_create", test_label_create},
        {"label_set_text", test_label_set_text},

        /* Slider */
        {"slider_create", test_slider_create},
        {"slider_set_value", test_slider_set_value},
        {"slider_set_range", test_slider_set_range},

        /* Checkbox */
        {"checkbox_create", test_checkbox_create},
        {"checkbox_set_checked", test_checkbox_set_checked},

        /* Progress */
        {"progress_create", test_progress_create},
        {"progress_set_value", test_progress_set_value},

        /* Panel */
        {"panel_create", test_panel_create},
        {"panel_content_size", test_panel_content_size},

        /* Dropdown */
        {"dropdown_create", test_dropdown_create},
        {"dropdown_set_selected", test_dropdown_set_selected},

        /* TextInput */
        {"textinput_create", test_textinput_create},
        {"textinput_set_text", test_textinput_set_text},

        /* ColorPicker */
        {"colorpicker_create", test_colorpicker_create},
        {"colorpicker_set_get_color", test_colorpicker_set_get_color},

        /* HBox/VBox */
        {"hbox_create", test_hbox_create},
        {"vbox_create", test_vbox_create},

        /* Visibility/Enabled */
        {"widget_visibility", test_widget_visibility},
        {"widget_enabled", test_widget_enabled},

        /* Hierarchy */
        {"widget_add_child", test_widget_add_child},
        {"widget_multiple_children", test_widget_multiple_children},

        /* Engine */
        {"engine_find", test_engine_find},
        {"engine_dirty_flag", test_engine_dirty_flag},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    return run_tests("Widget Unit Tests", tests, count);
}
