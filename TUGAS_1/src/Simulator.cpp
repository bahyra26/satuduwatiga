#include "Simulator.h"
#include <algorithm>

using MathUtils::Vec2;

Simulator::Simulator(const Field& field, const Goal& goal, const Ball& ball, const Robot& robot)
    : field_(field), goal_(goal), ball_(ball), robot_(robot) {
    buildWaypoints();
}

void Simulator::buildWaypoints() {
    const int colGap = 5;
    const int rowGap = 4;
    int k = 0;
    for (int row = 2; row < field_.getRows(); row += rowGap, ++k) {
        std::vector<int> cols;
        for (int c = 2; c < field_.getCols(); c += colGap) cols.push_back(c);
        if (k % 2 == 1) std::reverse(cols.begin(), cols.end());
        for (int c : cols) waypoints_.push_back(field_.toWorld(c, row));
    }
}

const char* Simulator::stateName(State s) {
    switch (s) {
        case State::Search:  return "SEARCH";
        case State::Align:   return "ALIGN";
        case State::Kick:    return "KICK";
        case State::Rolling: return "ROLLING";
        case State::Done:    return "DONE";
        case State::Failed:  return "FAILED";
    }
    return "?";
}

void Simulator::searchTick() {
    if (scanCount_ < 8) {
        ++scanCount_;
        if (robot_.scanStep(ball_)) state_ = State::Align;
        return;
    }
    if (wpIndex_ >= waypoints_.size()) {
        state_ = State::Failed;
        return;
    }
    if (!robot_.stepToward(waypoints_[wpIndex_], field_)) {
        ++wpIndex_;
        scanCount_ = 0;
        return;
    }
    if (robot_.canSee(ball_.getPosition())) state_ = State::Align;
}

void Simulator::alignTick() {
    if (robot_.alignToShoot(ball_, field_)) return;
    state_ = robot_.canKick(ball_, field_) ? State::Kick : State::Failed;
}

void Simulator::kickTick() {
    state_ = robot_.kickBall(ball_, field_) ? State::Rolling : State::Failed;
}

void Simulator::rollTick() {
    ball_.step(field_);
    if (!ball_.isMoving()) state_ = State::Done;
}

void Simulator::tick() {
    if (finished()) return;
    ++tick_;
    switch (state_) {
        case State::Search: searchTick(); break;
        case State::Align:  alignTick();  break;
        case State::Kick:   kickTick();   break;
        default: break;
    }
    if (state_ == State::Rolling) rollTick();
}

bool Simulator::finished() const {
    return state_ == State::Done || state_ == State::Failed;
}

void Simulator::render(std::ostream& os) const {
    Cell ballCell = field_.cellOf(ball_.getPosition());
    Cell robotCell = field_.cellOf(robot_.getPosition());

    for (int r = 0; r < field_.getRows(); ++r) {
        for (int c = 0; c < field_.getCols(); ++c) {
            char ch = '.';
            if (robot_.canSee(field_.toWorld(c, r))) ch = '@';
            if (goal_.isGoalCell(c, r, field_)) ch = '#';
            if (Cell{c, r} == ballCell) ch = 'O';
            if (Cell{c, r} == robotCell) ch = 'R';
            os << ch;
            if (c < field_.getCols() - 1) os << ' ';
        }
        os << "\n";
    }
}

void Simulator::run(int maxTicks, int renderEvery, std::ostream& os) {
    os << "== awal (t = 0 s) ==\n";
    render(os);

    while (!finished() && tick_ < maxTicks) {
        State before = state_;
        tick();
        bool changed = (state_ != before);
        if (changed || finished() || (renderEvery > 0 && tick_ % renderEvery == 0)) {
            os << "\n== t = " << getElapsed() << " s | " << stateName(state_)
               << " | robot (" << robot_.getPosition().x << ", " << robot_.getPosition().y
               << ") hadap " << robot_.getHeading() << " ==\n";
            render(os);
        }
    }

    os << "\n";
    if (state_ == State::Done)
        os << (scored() ? "GOL" : "tidak gol") << " setelah " << getElapsed() << " detik\n";
    else if (state_ == State::Failed)
        os << "gagal (bola tidak ketemu / tidak bisa ditendang) setelah " << getElapsed() << " detik\n";
    else
        os << "waktu habis (" << maxTicks << " tick)\n";
}

int Simulator::getTick() const { return tick_; }
double Simulator::getElapsed() const { return tick_ * TICK_SECONDS; }
Simulator::State Simulator::getState() const { return state_; }
bool Simulator::scored() const { return goal_.contains(ball_.getPosition(), field_); }
const Field& Simulator::getField() const { return field_; }
const Ball& Simulator::getBall() const { return ball_; }
const Robot& Simulator::getRobot() const { return robot_; }