#include "Field.h"
#include <cmath>
#include <stdexcept>

using MathUtils::Vec2;

namespace { constexpr double EPS = 1e-9; }

Field::Field(double width, double height, double cellSize, double goalHalfWidth)
    : width_(width), height_(height), cellSize_(cellSize),
      goalX_(width / 2.0), goalHalfWidth_(goalHalfWidth) {
    if (width <= 0 || height <= 0 || cellSize <= 0)
        throw std::invalid_argument("Ukuran lapangan harus positif");
    cols_ = static_cast<int>(std::lround(width_ / cellSize_)) + 1;
    rows_ = static_cast<int>(std::lround(height_ / cellSize_)) + 1;
}

int Field::getCols() const { return cols_; }
int Field::getRows() const { return rows_; }
double Field::getCellSize() const { return cellSize_; }
double Field::getGoalX() const { return goalX_; }

std::pair<int, int> Field::toGrid(const Vec2& p) const {
    int col = static_cast<int>(std::lround((p.x + width_ / 2.0) / cellSize_));
    int row = static_cast<int>(std::lround((height_ / 2.0 - p.y) / cellSize_));
    return {col, row};
}

Vec2 Field::toWorld(int col, int row) const {
    return {col * cellSize_ - width_ / 2.0, height_ / 2.0 - row * cellSize_};
}

bool Field::isValidCell(int col, int row) const {
    return col >= 0 && col < cols_ && row >= 0 && row < rows_;
}

bool Field::isInside(const Vec2& p) const {
    return std::fabs(p.x) <= width_ / 2.0 + EPS &&
           std::fabs(p.y) <= height_ / 2.0 + EPS;
}

bool Field::isInGoal(const Vec2& p) const {
    return p.x >= goalX_ - EPS && std::fabs(p.y) <= goalHalfWidth_ + EPS;
}

bool Field::isGoalCell(int col, int row) const {
    if (col != cols_ - 1 || !isValidCell(col, row)) return false;
    return std::fabs(toWorld(col, row).y) <= goalHalfWidth_ + EPS;
}