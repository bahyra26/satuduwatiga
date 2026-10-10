#pragma once
#include "MathUtils.h"
#include "Field.h"

class Ball {
private:
    MathUtils::Vec2 pos_;  // selalu di titik tengah petak
    int dx_ = 0;           // arah gerak per langkah: -1, 0, 1 (kolom, +kanan)
    int dy_ = 0;           // -1, 0, 1 (+atas)

public:
    Ball(const MathUtils::Vec2& pos, const Field& field);

    MathUtils::Vec2 getPosition() const;
    bool isMoving() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);  // throw kalau di luar lapangan, lalu snap ke titik tengah petak

    // sudut global (derajat), dibulatkan ke kelipatan 45: 0 = kanan, 90 = atas, 45 = miring kanan-atas
    void kick(double angleDeg);
    void stop();

    // geser satu petak sesuai arah; berhenti kalau petak berikutnya di luar lapangan
    // return true kalau bola berhasil bergerak
    bool step(const Field& field);
};