#include "Robot.h"
#include <stdexcept>

using MathUtils::Vec2;

Robot::Robot(const Vec2& pos, double heading, const Field& field, double fov, double range) {
    setPosition(pos, field);
    setHeading(heading);
    setVision(fov, range);
}


double Robot::getFov() const { return fov_; }
double Robot::getRange() const { return range_; }

void Robot::setVision(double fovDeg, double rangeM) {
    if (fovDeg <= 0 || fovDeg > 360 || rangeM <= 0)
        throw std::invalid_argument("Vision harus positif (fov maks 360)");
    fov_ = fovDeg;
    range_ = rangeM;
}

double Robot::distanceTo(const Vec2& target) const {
    return MathUtils::distance(pos_, target);
}

double Robot::relativeAngleTo(const Vec2& target) const {
    return MathUtils::normalizeAngle(MathUtils::bearing(pos_, target) - heading_);
}

bool Robot::canSee(const Vec2& target) const {
    double rel = relativeAngleTo(target);
    if (std::fabs(rel) > fov_ / 2.0 + 1e-9) return false;

    // jarak pandang diukur lurus ke depan (bukan melingkar), jadi bentuknya segitiga: 3, 5, 7, ...
    double d = distanceTo(target);
    double reach = (std::fabs(rel) <= 90.0) ? d * std::cos(MathUtils::toRad(rel)) : d;
    return reach <= range_ + 1e-9;
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