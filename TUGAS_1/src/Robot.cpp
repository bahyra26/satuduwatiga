#include "Robot.h"
#include <stdexcept>

using MathUtils::Vec2;

Robot::Robot(const Vec2& pos, double heading, const Field& field) {
    setPosition(pos, field);
    setHeading(heading);
}

Vec2 Robot::getPosition() const { return pos_; }
double Robot::getHeading() const { return heading_; }

void Robot::setPosition(const Vec2& p, const Field& field) {
    if (!field.isInside(p))
        throw std::out_of_range("Posisi robot di luar lapangan");
    pos_ = p;
}

void Robot::setHeading(double deg) {
    heading_ = MathUtils::normalizeAngle(deg);
}

double Robot::goalDistance(const Goal& goal) const {
    return MathUtils::distance(pos_, goal.center());
}

double Robot::goalBearing(const Goal& goal) const {
    return MathUtils::bearing(pos_, goal.center());
}

double Robot::goalRelativeAngle(const Goal& goal) const {
    return MathUtils::normalizeAngle(goalBearing(goal) - heading_);
}