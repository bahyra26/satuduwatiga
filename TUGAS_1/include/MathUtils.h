#pragma once
#include <cmath>

namespace MathUtils {

constexpr double PI  = 3.14159265358979323846;
constexpr double EPS = 1e-9;  // toleransi perbandingan double (satu sumber untuk semua file)

struct Vec2 {
    double x{0}, y{0};
};

// arah 8 penjuru pada grid: dx +kanan, dy +atas
struct Dir {
    int dx{0}, dy{0};
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

// bulatkan ke kelipatan 45 derajat terdekat, hasil di [-180, 180)
inline double snap45(double deg) {
    return normalizeAngle(std::round(deg / 45.0) * 45.0);
}

// sudut -> arah satu petak (sudut dibulatkan ke kelipatan 45 dulu)
inline Dir dirFromAngle(double deg) {
    double rad = toRad(snap45(deg));
    return {static_cast<int>(std::lround(std::cos(rad))),
            static_cast<int>(std::lround(std::sin(rad)))};
}

}  // namespace MathUtils
