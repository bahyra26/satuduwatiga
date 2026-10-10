#include "Striker.h"

Striker::Striker(const MathUtils::Vec2& pos, double heading, const Field& field,
                 const Goal& goal, double fov, double range)
    : Robot(pos, heading, field, goal, fov, range) {}

Robot::Action Striker::think(const Ball& ball, const Field& field) const {
    // bola tidak kelihatan dan tidak di depan hidung: cari dulu
    if (!canSee(ball.getPosition()) && !canKick(ball, field)) return Action::Search;

    // tendang hanya dari petak yang direncanakan planShot (bukan sekadar kebetulan menghadap bola)
    ShotPlan plan = planShot(ball, field);
    if (field.cellOf(getPosition()) == plan.standCell && canKick(ball, field))
        return Action::Kick;

    return Action::Align;
}