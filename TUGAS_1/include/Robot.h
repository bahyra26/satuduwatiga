#pragma once
#include "MathUtils.h"
#include "Field.h"
#include "Goal.h"

class Robot {
private:
    MathUtils::Vec2 pos_;
    double heading_;  // derajat
    double fov_;      // sudut pandang total (derajat)
    double range_;    // jarak pandang maksimum (meter)

public:
    Robot(const MathUtils::Vec2& pos, double heading, const Field& field,
          double fov = 90.0, double range = 3.0);

    double getFov() const;
    double getRange() const;
    void setVision(double fovDeg, double rangeM);
    bool canSee(const MathUtils::Vec2& target) const;
    double distanceTo(const MathUtils::Vec2& target) const;
    double relativeAngleTo(const MathUtils::Vec2& target) const;

    MathUtils::Vec2 getPosition() const;
    double getHeading() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);
    void setHeading(double deg);

    double goalDistance(const Goal& goal) const;
    double goalBearing(const Goal& goal) const;        // sudut global
    double goalRelativeAngle(const Goal& goal) const;  // relatif ke hadapan robot
};