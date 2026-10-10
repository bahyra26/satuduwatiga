#pragma once
#include "MathUtils.h"
#include "Field.h"

class Ball {
private:
    static constexpr double INITIAL_SPEED = 3.0;  
    static constexpr double DECELERATION  = 1.0;  

    MathUtils::Vec2 pos_;
    MathUtils::Dir dir_;
    double speed_ = 0.0;

public:
    Ball(const MathUtils::Vec2& pos, const Field& field);

    MathUtils::Vec2 getPosition() const;
    bool isMoving() const;
    double getSpeed() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);

    void kick(double angleDeg);
    void stop();

    bool step(const Field& field);
};
