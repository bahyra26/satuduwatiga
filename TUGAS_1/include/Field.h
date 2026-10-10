#pragma once
#include <utility>
#include "MathUtils.h"

using Cell = std::pair<int, int>;

// Konvensi: posisi, jarak, dan kecepatan dalam satuan meter (double)
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

    Cell toGrid(const MathUtils::Vec2& p) const;          
    Cell cellOf(const MathUtils::Vec2& p) const;        
    MathUtils::Vec2 toWorld(const Cell& c) const;        
    MathUtils::Vec2 toWorld(int col, int row) const;
    MathUtils::Vec2 snap(const MathUtils::Vec2& p) const; 

    // petak tetangga ke arah d (baris bertambah ke bawah, +y ke atas, ditangani di sini saja)
    Cell neighbor(const Cell& c, MathUtils::Dir d) const;

    double stepLength(MathUtils::Dir d) const;
    int stepsFor(double meters, MathUtils::Dir d) const;
    static int cellDistance(const Cell& a, const Cell& b);

    bool isValidCell(int col, int row) const;
    bool isInside(const MathUtils::Vec2& p) const;
};
