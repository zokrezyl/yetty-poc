/*
 * ygui_grid.c - Spatial grid for O(1) widget lookup
 */

#include "ygui_internal.h"

/*=============================================================================
 * Grid Cell Helpers
 *===========================================================================*/

static void cell_init(ygui_grid_cell_t* cell) {
    cell->widgets = NULL;
    cell->count = 0;
    cell->capacity = 0;
}

static void cell_destroy(ygui_grid_cell_t* cell) {
    free(cell->widgets);
    cell->widgets = NULL;
    cell->count = 0;
    cell->capacity = 0;
}

static void cell_clear(ygui_grid_cell_t* cell) {
    cell->count = 0;
}

static void cell_add(ygui_grid_cell_t* cell, ygui_widget_t* widget) {
    if (cell->count >= cell->capacity) {
        int new_cap = cell->capacity == 0 ? 4 : cell->capacity * 2;
        ygui_widget_t** new_arr = (ygui_widget_t**)realloc(
            cell->widgets, new_cap * sizeof(ygui_widget_t*));
        if (!new_arr) return;
        cell->widgets = new_arr;
        cell->capacity = new_cap;
    }
    cell->widgets[cell->count++] = widget;
}

static void cell_remove(ygui_grid_cell_t* cell, ygui_widget_t* widget) {
    for (int i = 0; i < cell->count; i++) {
        if (cell->widgets[i] == widget) {
            /* Shift remaining elements */
            for (int j = i; j < cell->count - 1; j++) {
                cell->widgets[j] = cell->widgets[j + 1];
            }
            cell->count--;
            return;
        }
    }
}

/*=============================================================================
 * Grid Implementation
 *===========================================================================*/

void ygui_grid_init(ygui_spatial_grid_t* grid, float width, float height, float cell_size) {
    grid->width = width;
    grid->height = height;
    grid->cell_size = cell_size;
    grid->cols = (int)(width / cell_size) + 1;
    grid->rows = (int)(height / cell_size) + 1;

    int total_cells = grid->cols * grid->rows;
    grid->cells = (ygui_grid_cell_t*)calloc(total_cells, sizeof(ygui_grid_cell_t));

    for (int i = 0; i < total_cells; i++) {
        cell_init(&grid->cells[i]);
    }
}

void ygui_grid_destroy(ygui_spatial_grid_t* grid) {
    if (!grid->cells) return;

    int total_cells = grid->cols * grid->rows;
    for (int i = 0; i < total_cells; i++) {
        cell_destroy(&grid->cells[i]);
    }
    free(grid->cells);
    grid->cells = NULL;
    grid->cols = 0;
    grid->rows = 0;
}

void ygui_grid_clear(ygui_spatial_grid_t* grid) {
    if (!grid->cells) return;

    int total_cells = grid->cols * grid->rows;
    for (int i = 0; i < total_cells; i++) {
        cell_clear(&grid->cells[i]);
    }
}

void ygui_grid_insert(ygui_spatial_grid_t* grid, ygui_widget_t* widget) {
    if (!grid->cells || !widget) return;

    float x0 = widget->effective_x;
    float y0 = widget->effective_y;
    float x1 = x0 + widget->w;
    float y1 = y0 + widget->h;

    int col0 = (int)(x0 / grid->cell_size);
    int row0 = (int)(y0 / grid->cell_size);
    int col1 = (int)(x1 / grid->cell_size);
    int row1 = (int)(y1 / grid->cell_size);

    col0 = col0 < 0 ? 0 : (col0 >= grid->cols ? grid->cols - 1 : col0);
    row0 = row0 < 0 ? 0 : (row0 >= grid->rows ? grid->rows - 1 : row0);
    col1 = col1 < 0 ? 0 : (col1 >= grid->cols ? grid->cols - 1 : col1);
    row1 = row1 < 0 ? 0 : (row1 >= grid->rows ? grid->rows - 1 : row1);

    for (int r = row0; r <= row1; r++) {
        for (int c = col0; c <= col1; c++) {
            int idx = r * grid->cols + c;
            cell_add(&grid->cells[idx], widget);
        }
    }

    /* Recursively insert children */
    for (ygui_widget_t* child = widget->first_child; child; child = child->next_sibling) {
        if (child->was_rendered) {
            ygui_grid_insert(grid, child);
        }
    }
}

void ygui_grid_remove(ygui_spatial_grid_t* grid, ygui_widget_t* widget) {
    if (!grid->cells || !widget) return;

    float x0 = widget->effective_x;
    float y0 = widget->effective_y;
    float x1 = x0 + widget->w;
    float y1 = y0 + widget->h;

    int col0 = (int)(x0 / grid->cell_size);
    int row0 = (int)(y0 / grid->cell_size);
    int col1 = (int)(x1 / grid->cell_size);
    int row1 = (int)(y1 / grid->cell_size);

    col0 = col0 < 0 ? 0 : (col0 >= grid->cols ? grid->cols - 1 : col0);
    row0 = row0 < 0 ? 0 : (row0 >= grid->rows ? grid->rows - 1 : row0);
    col1 = col1 < 0 ? 0 : (col1 >= grid->cols ? grid->cols - 1 : col1);
    row1 = row1 < 0 ? 0 : (row1 >= grid->rows ? grid->rows - 1 : row1);

    for (int r = row0; r <= row1; r++) {
        for (int c = col0; c <= col1; c++) {
            int idx = r * grid->cols + c;
            cell_remove(&grid->cells[idx], widget);
        }
    }

    /* Recursively remove children */
    for (ygui_widget_t* child = widget->first_child; child; child = child->next_sibling) {
        ygui_grid_remove(grid, child);
    }
}

ygui_widget_t* ygui_grid_query(const ygui_spatial_grid_t* grid, float x, float y) {
    if (!grid->cells) return NULL;

    int col = (int)(x / grid->cell_size);
    int row = (int)(y / grid->cell_size);

    if (col < 0 || col >= grid->cols || row < 0 || row >= grid->rows) {
        return NULL;
    }

    int idx = row * grid->cols + col;
    ygui_grid_cell_t* cell = &grid->cells[idx];

    /* Return topmost widget (last in list) that contains the point */
    for (int i = cell->count - 1; i >= 0; i--) {
        ygui_widget_t* w = cell->widgets[i];
        if (x >= w->effective_x && x < w->effective_x + w->w &&
            y >= w->effective_y && y < w->effective_y + w->h) {
            /* Check visibility */
            if (!(w->flags & YGUI_FLAG_VISIBLE) || (w->flags & YGUI_FLAG_DISABLED)) {
                continue;
            }
            return w;
        }
    }

    return NULL;
}
