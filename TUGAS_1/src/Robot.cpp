#include "Robot.h"
#include <stdexcept>
#include <algorithm>

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
    auto [c, r] = field.toGrid(p);
    c = std::clamp(c, 0, field.getCols() - 1);
    r = std::clamp(r, 0, field.getRows() - 1);
    pos_ = field.toWorld(c, r);   // selalu di tengah petak
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
void Robot::rotate(double deg) {
    setHeading(heading_ + deg);
}

bool Robot::scanStep(const Ball& ball, double stepDeg) {
    if (canSee(ball.getPosition())) return true;
    rotate(stepDeg);
    return canSee(ball.getPosition());
}

bool Robot::stepToBall(const Ball& ball, const Field& field) {
    Vec2 target = ball.getPosition();
    if (distanceTo(target) <= field.getCellSize() * 1.5 + 1e-9) return false;  // sudah bersebelahan

    double snapped = std::round(MathUtils::bearing(pos_, target) / 45.0) * 45.0;
    double rad = MathUtils::toRad(snapped);
    int dx = static_cast<int>(std::lround(std::cos(rad)));
    int dy = static_cast<int>(std::lround(std::sin(rad)));

    auto [c, r] = field.toGrid(pos_);
    int nc = c + dx;
    int nr = r - dy;  // baris bertambah ke bawah, sedangkan +y ke atas
    if (!field.isValidCell(nc, nr)) return false;

    pos_ = field.toWorld(nc, nr);
    setHeading(snapped);
    return true;
}

bool Robot::kickBall(Ball& ball, const Goal& goal, const Field& field) {
    if (distanceTo(ball.getPosition()) > field.getCellSize() * 1.5 + 1e-9) return false;

    // arah tendangan dibulatkan ke kelipatan 45, robot ikut menghadap arah itu
    double angle = std::round(MathUtils::bearing(ball.getPosition(), goal.center()) / 45.0) * 45.0;
    setHeading(angle);
    ball.kick(angle);
    return true;
}