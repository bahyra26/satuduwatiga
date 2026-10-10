#include "Ball.h"
#include <stdexcept>

using MathUtils::Vec2;

Ball::Ball(const Vec2& pos, const Field& field) {
    setPosition(pos, field);
}

Vec2 Ball::getPosition() const { return pos_; }
bool Ball::isMoving() const {
    return speed_ > MathUtils::EPS && (dir_.dx != 0 || dir_.dy != 0);
}
double Ball::getSpeed() const { return speed_; }

void Ball::setPosition(const Vec2& p, const Field& field) {
    if (!field.isInside(p))
        throw std::out_of_range("Posisi bola di luar lapangan");
    pos_ = field.snap(p);
    stop();
}

void Ball::kick(double angleDeg) {
    dir_ = MathUtils::dirFromAngle(angleDeg);
    speed_ = INITIAL_SPEED;
}

void Ball::stop() { dir_ = {0, 0}; speed_ = 0.0; }

bool Ball::step(const Field& field) {
    if (!isMoving()) return false;

    Cell cell = field.cellOf(pos_);
    int budget = field.stepsFor(speed_, dir_);
    int moved = 0;
    bool hitWall = false;

    for (int i = 0; i < budget; ++i) {
        Cell next = field.neighbor(cell, dir_);
        if (!field.isValidCell(next.first, next.second)) {
            hitWall = true;
            break;
        }
        cell = next;
        ++moved;
    }

    pos_ = field.toWorld(cell);
    speed_ -= DECELERATION;
    if (hitWall || speed_ <= MathUtils::EPS) stop();

    return moved > 0;
}
