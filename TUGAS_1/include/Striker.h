#pragma once
#include "Robot.h"

class Striker : public Robot {
public:
    Striker(const MathUtils::Vec2& pos, double heading, const Field& field,
            const Goal& goal = Goal(), double fov = DEFAULT_FOV, double range = DEFAULT_RANGE);

    Action think(const Ball& ball, const Field& field) const override;
};