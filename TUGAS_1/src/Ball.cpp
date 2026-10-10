#include "Ball.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

using MathUtils::Vec2;

Ball::Ball(const Vec2& pos, const Field& field) {
    setPosition(pos, field);
}

Vec2 Ball::getPosition() const { return pos_; }
bool Ball::isMoving() const { return dx_ != 0 || dy_ != 0; }

void Ball::setPosition(const Vec2& p, const Field& field) {
    if (!field.isInside(p))
        throw std::out_of_range("Posisi bola di luar lapangan");
    auto [c, r] = field.toGrid(p);
    c = std::clamp(c, 0, field.getCols() - 1);  // titik di garis tepi kanan/bawah
    r = std::clamp(r, 0, field.getRows() - 1);
    pos_ = field.toWorld(c, r);
    stop();
}

void Ball::kick(double angleDeg) {
    double snapped = std::round(MathUtils::normalizeAngle(angleDeg) / 45.0) * 45.0;
    double rad = MathUtils::toRad(snapped);
    dx_ = static_cast<int>(std::lround(std::cos(rad)));
    dy_ = static_cast<int>(std::lround(std::sin(rad)));
}

void Ball::stop() { dx_ = dy_ = 0; }

bool Ball::step(const Field& field) {
    if (!isMoving()) return false;

    auto [c, r] = field.toGrid(pos_);
    int nc = c + dx_;
    int nr = r - dy_;  // baris bertambah ke bawah, sedangkan +y ke atas
    if (!field.isValidCell(nc, nr)) {
        stop();
        return false;
    }
    pos_ = field.toWorld(nc, nr);
    return true;
}