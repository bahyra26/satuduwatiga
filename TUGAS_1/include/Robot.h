#pragma once
#include "MathUtils.h"
#include "Field.h"
#include "Goal.h"
#include "Ball.h"

class Robot {
private:
    MathUtils::Vec2 pos_;
    double heading_;  // derajat
    double fov_;      // sudut pandang total (derajat)
    double range_;    // jarak pandang maksimum (meter)

public:
    Robot(const MathUtils::Vec2& pos, double heading, const Field& field,
          double fov = 90.0, double range = 3.0);

    double getFov() const;
    double getRange() const;
    void setVision(double fovDeg, double rangeM);
    bool canSee(const MathUtils::Vec2& target) const;
    double distanceTo(const MathUtils::Vec2& target) const;
    double relativeAngleTo(const MathUtils::Vec2& target) const;

    MathUtils::Vec2 getPosition() const;
    double getHeading() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);
    void setHeading(double deg);

    double goalDistance(const Goal& goal) const;
    double goalBearing(const Goal& goal) const;        // sudut global
    double goalRelativeAngle(const Goal& goal) const;  // relatif ke hadapan robot

    // putar hadapan sebesar deg derajat (+ berlawanan arah jarum jam)
    void rotate(double deg);

    // scanning: kalau bola belum kelihatan, putar stepDeg derajat
    // return true kalau bola kelihatan (setelah diputar)
    bool scanStep(const Ball& ball, double stepDeg = 15.0);

    // maju satu petak ke arah bola (8 arah), berhenti kalau sudah bersebelahan dengan bola
    // return true kalau robot berhasil bergerak
    bool stepToBall(const Ball& ball, const Field& field);

    // tendang bola ke arah gawang, hanya kalau bola bersebelahan dengan robot
    // return true kalau berhasil menendang
    bool kickBall(Ball& ball, const Goal& goal, const Field& field);
};