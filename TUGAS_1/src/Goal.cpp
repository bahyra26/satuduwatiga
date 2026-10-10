#include "Goal.h"
#include <cmath>

using MathUtils::Vec2;

Goal::Goal(double x, double halfWidth) : x_(x), halfWidth_(halfWidth) {}

Vec2 Goal::center() const { return {x_, 0.0}; }

bool Goal::contains(const Vec2& p, const Field& field) const {
    // titik tengah petak terakhir ada di x_ - cellSize/2, jadi batas kiri digeser setengah petak
    return p.x >= x_ - field.getCellSize() / 2.0 - MathUtils::EPS &&
           std::fabs(p.y) <= halfWidth_ + MathUtils::EPS;
}

bool Goal::isGoalCell(int col, int row, const Field& field) const {
    return field.isValidCell(col, row) && contains(field.toWorld(col, row), field);
}
