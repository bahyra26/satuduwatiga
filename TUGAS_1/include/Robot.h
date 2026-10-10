#pragma once
#include "MathUtils.h"
#include "Field.h"
#include "Goal.h"

class Robot {
private:
    MathUtils::Vec2 pos_;
    double heading_;  // derajat

public:
    Robot(const MathUtils::Vec2& pos, double heading, const Field& field);

    MathUtils::Vec2 getPosition() const;
    double getHeading() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);
    void setHeading(double deg);

    double goalDistance(const Goal& goal) const;
    double goalBearing(const Goal& goal) const;        // sudut global
    double goalRelativeAngle(const Goal& goal) const;  // relatif ke hadapan robot
};