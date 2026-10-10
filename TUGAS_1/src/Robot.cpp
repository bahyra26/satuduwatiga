#include "Robot.h"
#include <algorithm>
#include <stdexcept>

using MathUtils::Vec2;

Robot::Robot(const Vec2& pos, double heading, const Field& field, const Goal& goal,
             double fov, double range)
    : goal_(goal) {
    setPosition(pos, field);
    setHeading(heading);
    setVision(fov, range);
}

double Robot::getFov() const { return fov_; }
double Robot::getRange() const { return range_; }

void Robot::setVision(double fovDeg, double rangeM) {
    if (fovDeg <= 0 || fovDeg > 360 || rangeM <= 0)
        throw std::invalid_argument("Vision harus positif (fov maks 360)");
    fov_ = fovDeg;
    range_ = rangeM;
}

double Robot::distanceTo(const Vec2& target) const {
    return MathUtils::distance(pos_, target);
}

double Robot::relativeAngleTo(const Vec2& target) const {
    return MathUtils::normalizeAngle(MathUtils::bearing(pos_, target) - heading_);
}

bool Robot::canSee(const Vec2& target) const {
    double rel = relativeAngleTo(target);
    if (std::fabs(rel) > fov_ / 2.0 + MathUtils::EPS) return false;

    // jarak pandang diukur lurus ke depan (bukan melingkar), jadi bentuknya segitiga: 3, 5, 7, ...
    double d = distanceTo(target);
    double reach = (std::fabs(rel) <= 90.0) ? d * std::cos(MathUtils::toRad(rel)) : d;
    return reach <= range_ + MathUtils::EPS;
}

Vec2 Robot::getPosition() const { return pos_; }
double Robot::getHeading() const { return heading_; }

void Robot::setPosition(const Vec2& p, const Field& field) {
    if (!field.isInside(p))
        throw std::out_of_range("Posisi robot di luar lapangan");
    pos_ = field.snap(p);  // selalu di tengah petak
}

void Robot::setHeading(double deg) {
    double units = deg / 45.0;
    if (std::fabs(units - std::round(units)) > MathUtils::EPS)
        throw std::invalid_argument("Hadapan robot harus kelipatan 45 derajat");
    heading_ = MathUtils::normalizeAngle(deg);
}

const Goal& Robot::getGoal() const { return goal_; }
void Robot::setGoal(const Goal& goal) { goal_ = goal; }

Cell Robot::frontCell(const Field& field) const {
    return field.neighbor(field.cellOf(pos_), MathUtils::dirFromAngle(heading_));
}

double Robot::goalDistance() const { return distanceTo(goal_.center()); }

double Robot::goalBearing() const {
    return MathUtils::bearing(pos_, goal_.center());
}

double Robot::goalRelativeAngle() const {
    return relativeAngleTo(goal_.center());
}

void Robot::rotate(double deg) {
    setHeading(heading_ + deg);
}

bool Robot::scanStep(const Ball& ball, double stepDeg) {
    if (canSee(ball.getPosition())) return true;
    rotate(stepDeg);
    return canSee(ball.getPosition());
}

bool Robot::scanFull(const Ball& ball, double stepDeg) {
    int n = static_cast<int>(std::lround(360.0 / stepDeg));
    for (int i = 0; i < n; ++i)
        if (scanStep(ball, stepDeg)) return true;
    return false;
}

bool Robot::stepAlong(double angleDeg, const Field& field) {
    double snapped = MathUtils::snap45(angleDeg);
    Cell next = field.neighbor(field.cellOf(pos_), MathUtils::dirFromAngle(snapped));
    if (!field.isValidCell(next.first, next.second)) return false;

    pos_ = field.toWorld(next);
    setHeading(snapped);
    return true;
}

bool Robot::stepToward(const Vec2& target, const Field& field, const Cell* avoid) {
    Vec2 goalPoint = field.snap(target);
    Cell me = field.cellOf(pos_);
    if (me == field.cellOf(goalPoint)) return false;  // sudah di petak target

    auto blocked = [&](double angle) {
        Cell n = field.neighbor(me, MathUtils::dirFromAngle(angle));
        return !field.isValidCell(n.first, n.second) || (avoid && n == *avoid);
    };

    double angle = MathUtils::snap45(MathUtils::bearing(pos_, goalPoint));
    if (!blocked(angle)) return stepAlong(angle, field);

    // terhalang: belok ±45 lalu ±90, ambil langkah yang paling dekat ke target
    bool found = false;
    double bestAngle = 0.0, bestDist = 0.0;
    for (double off : {45.0, -45.0, 90.0, -90.0}) {
        double a = MathUtils::normalizeAngle(angle + off);
        if (blocked(a)) continue;
        Cell n = field.neighbor(me, MathUtils::dirFromAngle(a));
        double d = MathUtils::distance(field.toWorld(n), goalPoint);
        if (!found || d < bestDist - MathUtils::EPS) {
            found = true;
            bestAngle = a;
            bestDist = d;
        }
    }
    return found && stepAlong(bestAngle, field);
}

Robot::ShotPlan Robot::planShot(const Ball& ball, const Field& field) const {
    Cell ballCell = field.cellOf(ball.getPosition());
    double toGoal = MathUtils::bearing(ball.getPosition(), goal_.center());

    ShotPlan best{ballCell, MathUtils::snap45(toGoal), false};
    double bestDiff = 0.0;
    bool have = false;

    for (int i = 0; i < 8; ++i) {
        double angle = MathUtils::normalizeAngle(45.0 * i);
        MathUtils::Dir d = MathUtils::dirFromAngle(angle);
        Cell stand = field.neighbor(ballCell, {-d.dx, -d.dy});  // di belakang bola
        if (!field.isValidCell(stand.first, stand.second)) continue;

        Ball sim = ball;  // simulasi tendangan pada salinan bola
        sim.kick(angle);
        while (sim.step(field)) {}
        bool scores = goal_.contains(sim.getPosition(), field);
        double diff = std::fabs(MathUtils::normalizeAngle(angle - toGoal));

        bool better = !have || (scores && !best.scores) ||
                      (scores == best.scores && diff < bestDiff - MathUtils::EPS);
        if (better) {
            best = {stand, angle, scores};
            bestDiff = diff;
            have = true;
        }
    }
    return best;
}

bool Robot::alignToShoot(const Ball& ball, const Field& field) {
    ShotPlan plan = planShot(ball, field);
    Cell me = field.cellOf(pos_);
    Cell ballCell = field.cellOf(ball.getPosition());
    if (me == ballCell) return false;  // satu petak dengan bola: tidak bisa diselesaikan

    if (me != plan.standCell)
        return stepToward(field.toWorld(plan.standCell), field, &ballCell);

    // sudah di posisi tendang: hadap searah tendangan (otomatis menghadap bola)
    double want = MathUtils::snap45(plan.angle);
    if (std::fabs(MathUtils::normalizeAngle(want - heading_)) < MathUtils::EPS) return false;
    setHeading(want);
    return true;
}

bool Robot::canKick(const Ball& ball, const Field& field) const {
    return field.cellOf(ball.getPosition()) == frontCell(field);
}

bool Robot::kickBall(Ball& ball, const Field& field) {
    if (!canKick(ball, field)) return false;

    // arah rencana relatif hadapan robot, dibatasi -45 / 0 / +45
    double rel = MathUtils::normalizeAngle(planShot(ball, field).angle - heading_);
    double offset = std::clamp(MathUtils::snap45(rel), -45.0, 45.0);

    ball.kick(heading_ + offset);
    return true;
}