#pragma once
#include <cmath>
#include <stdexcept>
#include "MathUtils.h"

class Sensor {
private:
    double fov_;    // sudut pandang total (derajat)
    double range_;  // jarak pandang maksimum lurus ke depan (meter)

public:
    Sensor(double fovDeg, double rangeM) : fov_(90.0), range_(1.5) {
        setVision(fovDeg, rangeM);
    }

    double getFov() const { return fov_; }
    double getRange() const { return range_; }

    void setVision(double fovDeg, double rangeM) {
        if (fovDeg <= 0 || fovDeg > 360 || rangeM <= 0)
            throw std::invalid_argument("Vision harus positif (fov maks 360)");
        fov_ = fovDeg;
        range_ = rangeM;
    }

    // apakah target kelihatan dari posisi origin yang menghadap heading (derajat)?
    bool canSee(const MathUtils::Vec2& origin, double heading, const MathUtils::Vec2& target) const {
        double rel = MathUtils::normalizeAngle(MathUtils::bearing(origin, target) - heading);
        if (std::fabs(rel) > fov_ / 2.0 + MathUtils::EPS) return false;

        // jarak pandang diukur lurus ke depan (bukan melingkar), jadi bentuknya segitiga: 3, 5, 7, ...
        double d = MathUtils::distance(origin, target);
        double reach = (std::fabs(rel) <= 90.0) ? d * std::cos(MathUtils::toRad(rel)) : d;
        return reach <= range_ + MathUtils::EPS;
    }
};