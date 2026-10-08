#pragma once
#include "MathUtils.h"
#include "Field.h"

class Goal {
private:
    double x_;
    double halfWidth_;

public:
    Goal(double x = 4.5, double halfWidth = 1.5);

    MathUtils::Vec2 center() const;
    bool contains(const MathUtils::Vec2& p) const;
    bool isGoalCell(int col, int row, const Field& field) const;
};