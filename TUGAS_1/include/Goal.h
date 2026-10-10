#pragma once
#include "MathUtils.h"
#include "Field.h"

class Goal {
private:
    double x_;          // posisi garis gawang (meter)
    double halfWidth_;  // setengah lebar gawang (meter); lebar total 3 m -> 1.5

public:
    Goal(double x = 4.5, double halfWidth = 1.5);

    MathUtils::Vec2 center() const;

    // satu-satunya definisi "gol": titik ada di petak garis gawang (kolom terakhir)
    // dan di dalam lebar gawang
    bool contains(const MathUtils::Vec2& p, const Field& field) const;

    // untuk render, memakai contains() yang sama
    bool isGoalCell(int col, int row, const Field& field) const;
};
