#pragma once
#include <cmath>

namespace MathUtils {

constexpr double PI = 3.14159265358979323846;

struct Vec2 {
    double x{0}, y{0};
};

inline double toRad(double deg) { return deg * PI / 180.0; }
inline double toDeg(double rad) { return rad * 180.0 / PI; }

// hasil di rentang [-180, 180)
inline double normalizeAngle(double deg) {
    deg = std::fmod(deg + 180.0, 360.0);
    if (deg < 0) deg += 360.0;
    return deg - 180.0;
}

inline double distance(const Vec2& a, const Vec2& b) {
    return std::hypot(b.x - a.x, b.y - a.y);
}

// sudut global dari a ke b (derajat)
inline double bearing(const Vec2& a, const Vec2& b) {
    return toDeg(std::atan2(b.y - a.y, b.x - a.x));
}

}  // namespace MathUtils