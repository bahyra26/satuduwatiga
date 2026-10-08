#pragma once
#include "MathUtils.h"
#include "Field.h"

class Ball {
private:
    MathUtils::Vec2 pos_;

public:
    Ball(const MathUtils::Vec2& pos, const Field& field);

    MathUtils::Vec2 getPosition() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);  // throw kalau di luar lapangan
};
