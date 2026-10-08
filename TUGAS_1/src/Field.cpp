#include "Field.h"
#include <cmath>
#include <stdexcept>

using MathUtils::Vec2;

namespace { constexpr double EPS = 1e-9; }

Field::Field(double width, double height, double cellSize)
    : width_(width), height_(height), cellSize_(cellSize) {
    if (width <= 0 || height <= 0 || cellSize <= 0)
        throw std::invalid_argument("Ukuran lapangan harus positif");
    cols_ = static_cast<int>(std::lround(width_ / cellSize_));   // 18
    rows_ = static_cast<int>(std::lround(height_ / cellSize_));  // 12
}

int Field::getCols() const { return cols_; }
int Field::getRows() const { return rows_; }
double Field::getCellSize() const { return cellSize_; }

std::pair<int, int> Field::toGrid(const Vec2& p) const {
    int col = static_cast<int>(std::floor((p.x + width_ / 2.0) / cellSize_ + EPS));
    int row = static_cast<int>(std::floor((height_ / 2.0 - p.y) / cellSize_ + EPS));
    return {col, row};
}

Vec2 Field::toWorld(int col, int row) const {
    return {(col + 0.5) * cellSize_ - width_ / 2.0,
            height_ / 2.0 - (row + 0.5) * cellSize_};
}

bool Field::isValidCell(int col, int row) const {
    return col >= 0 && col < cols_ && row >= 0 && row < rows_;
}

bool Field::isInside(const Vec2& p) const {
    return std::fabs(p.x) <= width_ / 2.0 + EPS &&
           std::fabs(p.y) <= height_ / 2.0 + EPS;
}