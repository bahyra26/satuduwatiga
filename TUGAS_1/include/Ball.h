#pragma once
#include "MathUtils.h"
#include "Field.h"

class Ball {
private:
    static constexpr int INITIAL_SPEED = 3;  // m per tick saat ditendang
    static constexpr int DECELERATION  = 1;  // pengurangan kecepatan per tick

    MathUtils::Vec2 pos_;  // selalu di titik tengah petak
    int dx_ = 0;           // arah gerak per langkah: -1, 0, 1 (kolom, +kanan)
    int dy_ = 0;           // -1, 0, 1 (+atas)
    int speed_ = 0;        // kecepatan saat ini (petak per tick)

public:
    Ball(const MathUtils::Vec2& pos, const Field& field);

    MathUtils::Vec2 getPosition() const;
    bool isMoving() const;
    int getSpeed() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);

    // sudut global (derajat), dibulatkan ke kelipatan 45
    // kecepatan awal di-reset ke INITIAL_SPEED
    void kick(double angleDeg);
    void stop();

    // satu tick: bola bergerak sejauh speed_ petak, lalu speed_ berkurang DECELERATION
    // berhenti kalau kecepatan habis atau menabrak batas lapangan
    // return true kalau bola bergerak minimal satu petak
    bool step(const Field& field);
};