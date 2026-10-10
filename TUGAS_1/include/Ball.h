#pragma once
#include "MathUtils.h"
#include "Field.h"

class Ball {
private:
    static constexpr double INITIAL_SPEED = 3.0;  
    static constexpr double DECELERATION  = 1.0;  

    MathUtils::Vec2 pos_;  // selalu di titik tengah petak (meter)
    MathUtils::Dir dir_;   // arah gerak 8 penjuru
    double speed_ = 0.0;   // kecepatan saat ini (meter per tick)

public:
    Ball(const MathUtils::Vec2& pos, const Field& field);

    MathUtils::Vec2 getPosition() const;
    bool isMoving() const;
    double getSpeed() const;  // meter per tick
    void setPosition(const MathUtils::Vec2& p, const Field& field);

    // sudut global (derajat), dibulatkan ke kelipatan 45
    // kecepatan awal di-reset ke INITIAL_SPEED
    void kick(double angleDeg);
    void stop();

    // satu tick: bola bergerak sejauh speed_ meter (= speed_ / cellSize petak),
    // lalu speed_ berkurang DECELERATION. Tendangan: 3 m, 2 m, 1 m (total 6 m = 12 petak)
    // berhenti kalau kecepatan habis atau menabrak batas lapangan
    // return true kalau bola bergerak minimal satu petak
    bool step(const Field& field);
};
