#pragma once
#include <utility>
#include "MathUtils.h"

class Field {
private:
    double width_;
    double height_;
    double cellSize_;
    int cols_;
    int rows_;

public:
    Field(double width = 9.0, double height = 6.0, double cellSize = 0.5);

    int getCols() const;
    int getRows() const;
    double getCellSize() const;

    std::pair<int, int> toGrid(const MathUtils::Vec2& p) const;  // (col, row)
    MathUtils::Vec2 toWorld(int col, int row) const;             // titik tengah petak

    bool isValidCell(int col, int row) const;
    bool isInside(const MathUtils::Vec2& p) const;
};

// dibuat dengan bantuan AI