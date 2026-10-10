#include "Striker.h"

Striker::Striker(const MathUtils::Vec2& pos, double heading, const Field& field,
                 const Goal& goal, double fov, double range)
    : Robot(pos, heading, field, goal, fov, range) {}

Robot::Action Striker::think(const Ball& ball, const Field& field) const {
    if (!canSee(ball.getPosition()) && !canKick(ball, field)) return Action::Search;

    ShotPlan plan = planShot(ball, field);
    if (field.cellOf(getPosition()) == plan.standCell && canKick(ball, field))
        return Action::Kick;

    return Action::Align;
}