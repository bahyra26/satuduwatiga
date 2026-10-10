#include "Ball.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

using MathUtils::Vec2;

Ball::Ball(const Vec2& pos, const Field& field) {
    setPosition(pos, field);
}

Vec2 Ball::getPosition() const { return pos_; }
bool Ball::isMoving() const { return speed_ > 0 && (dx_ != 0 || dy_ != 0); }
int Ball::getSpeed() const { return speed_; }

void Ball::setPosition(const Vec2& p, const Field& field) {
    if (!field.isInside(p))
        throw std::out_of_range("Posisi bola di luar lapangan");
    auto [c, r] = field.toGrid(p);
    c = std::clamp(c, 0, field.getCols() - 1);
    r = std::clamp(r, 0, field.getRows() - 1);
    pos_ = field.toWorld(c, r);
    stop();
}

void Ball::kick(double angleDeg) {
    double snapped = std::round(MathUtils::normalizeAngle(angleDeg) / 45.0) * 45.0;
    double rad = MathUtils::toRad(snapped);
    dx_ = static_cast<int>(std::lround(std::cos(rad)));
    dy_ = static_cast<int>(std::lround(std::sin(rad)));
    speed_ = INITIAL_SPEED;
}

void Ball::stop() { dx_ = dy_ = 0; speed_ = 0; }

bool Ball::step(const Field& field) {
    if (!isMoving()) return false;

    auto [c, r] = field.toGrid(pos_);
    int moved = 0;
    bool hitWall = false;

    // gerak petak demi petak sebanyak speed_
    for (int i = 0; i < speed_; ++i) {
        int nc = c + dx_;
        int nr = r - dy_;  // baris bertambah ke bawah, sedangkan +y ke atas
        if (!field.isValidCell(nc, nr)) {
            hitWall = true;
            break;
        }
        c = nc;
        r = nr;
        ++moved;
    }

    pos_ = field.toWorld(c, r);

    if (hitWall) {
        stop();
    } else {
        speed_ = std::max(0, speed_ - DECELERATION);
        if (speed_ == 0) stop();
    }

    return moved > 0;
}