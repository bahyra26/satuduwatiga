#include "Goal.h"
#include <cmath>

using MathUtils::Vec2;

namespace { constexpr double EPS = 1e-9; }

Goal::Goal(double x, double halfWidth) : x_(x), halfWidth_(halfWidth) {}

Vec2 Goal::center() const { return {x_, 0.0}; }

bool Goal::contains(const Vec2& p) const {
    return p.x >= x_ - EPS && std::fabs(p.y) <= halfWidth_ + EPS;
}

bool Goal::isGoalCell(int col, int row, const Field& field) const {
    if (col != field.getCols() - 1 || !field.isValidCell(col, row)) return false;
    return std::fabs(field.toWorld(col, row).y) < halfWidth_;
}