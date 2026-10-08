#include "Ball.h"
#include <stdexcept>

using MathUtils::Vec2;

Ball::Ball(const Vec2& pos, const Field& field) {
    setPosition(pos, field);
}

Vec2 Ball::getPosition() const { return pos_; }

void Ball::setPosition(const Vec2& p, const Field& field) {
    if (!field.isInside(p))
        throw std::out_of_range("Posisi bola di luar lapangan");
    pos_ = p;
}