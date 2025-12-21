#include "Grid.h"
#include <algorithm>
#include <cstring>

namespace yetty {

Grid::Grid(uint32_t cols, uint32_t rows)
    : cols_(cols), rows_(rows), cells_(cols * rows) {
    clear();
}

void Grid::resize(uint32_t cols, uint32_t rows) {
    if (cols == cols_ && rows == rows_) return;

    std::vector<Cell> newCells(cols * rows);

    // Copy existing content
    uint32_t copyRows = std::min(rows, rows_);
    uint32_t copyCols = std::min(cols, cols_);

    for (uint32_t r = 0; r < copyRows; ++r) {
        for (uint32_t c = 0; c < copyCols; ++c) {
            newCells[r * cols + c] = cells_[r * cols_ + c];
        }
    }

    cols_ = cols;
    rows_ = rows;
    cells_ = std::move(newCells);
}

void Grid::clear() {
    Cell defaultCell;
    std::fill(cells_.begin(), cells_.end(), defaultCell);
}

void Grid::setCell(uint32_t col, uint32_t row, const Cell& cell) {
    if (col >= cols_ || row >= rows_) return;
    cells_[row * cols_ + col] = cell;
}

void Grid::setChar(uint32_t col, uint32_t row, uint32_t codepoint) {
    if (col >= cols_ || row >= rows_) return;
    cells_[row * cols_ + col].codepoint = codepoint;
}

void Grid::setFgColor(uint32_t col, uint32_t row, const glm::vec4& color) {
    if (col >= cols_ || row >= rows_) return;
    cells_[row * cols_ + col].fgColor = color;
}

void Grid::setBgColor(uint32_t col, uint32_t row, const glm::vec4& color) {
    if (col >= cols_ || row >= rows_) return;
    cells_[row * cols_ + col].bgColor = color;
}

const Cell& Grid::getCell(uint32_t col, uint32_t row) const {
    static Cell defaultCell;
    if (col >= cols_ || row >= rows_) return defaultCell;
    return cells_[row * cols_ + col];
}

Cell& Grid::getCell(uint32_t col, uint32_t row) {
    static Cell defaultCell;
    if (col >= cols_ || row >= rows_) return defaultCell;
    return cells_[row * cols_ + col];
}

void Grid::writeString(uint32_t col, uint32_t row, const char* str, const glm::vec4& fgColor) {
    if (!str || row >= rows_) return;

    uint32_t c = col;
    while (*str && c < cols_) {
        // Simple ASCII handling - for full Unicode would need proper UTF-8 decoding
        Cell cell;
        cell.codepoint = static_cast<uint8_t>(*str);
        cell.fgColor = fgColor;
        setCell(c, row, cell);
        ++str;
        ++c;
    }
}

} // namespace yetty
