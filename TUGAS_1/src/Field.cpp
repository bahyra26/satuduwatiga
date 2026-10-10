#include "Field.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

using MathUtils::Vec2;

Field::Field(double width, double height, double cellSize)
    : width_(width), height_(height), cellSize_(cellSize) {
    if (width <= 0 || height <= 0 || cellSize <= 0)
        throw std::invalid_argument("Ukuran lapangan harus positif");
    cols_ = static_cast<int>(std::lround(width_ / cellSize_));
    rows_ = static_cast<int>(std::lround(height_ / cellSize_));
}

int Field::getCols() const { return cols_; }
int Field::getRows() const { return rows_; }
double Field::getCellSize() const { return cellSize_; }

Cell Field::toGrid(const Vec2& p) const {
    int col = static_cast<int>(std::floor((p.x + width_ / 2.0) / cellSize_ + MathUtils::EPS));
    int row = static_cast<int>(std::floor((height_ / 2.0 - p.y) / cellSize_ + MathUtils::EPS));
    return {col, row};
}

Cell Field::cellOf(const Vec2& p) const {
    auto [c, r] = toGrid(p);
    return {std::clamp(c, 0, cols_ - 1), std::clamp(r, 0, rows_ - 1)};
}

Vec2 Field::toWorld(int col, int row) const {
    return {(col + 0.5) * cellSize_ - width_ / 2.0,
            height_ / 2.0 - (row + 0.5) * cellSize_};
}

Vec2 Field::toWorld(const Cell& c) const { return toWorld(c.first, c.second); }

Vec2 Field::snap(const Vec2& p) const { return toWorld(cellOf(p)); }

Cell Field::neighbor(const Cell& c, MathUtils::Dir d) const {
    return {c.first + d.dx, c.second - d.dy};
}

double Field::stepLength(MathUtils::Dir d) const {
    return cellSize_ * std::hypot(d.dx, d.dy);  // lurus = 0.5 m, diagonal = 0.5*√2 ≈ 0.707 m
}

int Field::stepsFor(double meters, MathUtils::Dir d) const {
    double len = stepLength(d);
    if (len <= MathUtils::EPS) return 0;
    return static_cast<int>(std::floor(meters / len + MathUtils::EPS));
}

int Field::cellDistance(const Cell& a, const Cell& b) {
    return std::max(std::abs(a.first - b.first), std::abs(a.second - b.second));
}

bool Field::isValidCell(int col, int row) const {
    return col >= 0 && col < cols_ && row >= 0 && row < rows_;
}

bool Field::isInside(const Vec2& p) const {
    return std::fabs(p.x) <= width_ / 2.0 + MathUtils::EPS &&
           std::fabs(p.y) <= height_ / 2.0 + MathUtils::EPS;
}
