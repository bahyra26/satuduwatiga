#pragma once
#include <cstddef>
#include <iostream>
#include <vector>
#include "MathUtils.h"
#include "Field.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"

// Pengelola game loop. 1 tick = 1 detik; di tiap tick robot melakukan SATU aksi
// (putar 45 derajat / maju 1 petak / tendang), lalu bola bergerak 1 langkah kalau sedang menggelinding.
class Simulator {
public:
    static constexpr double TICK_SECONDS = 1.0;

    enum class State { Search, Align, Kick, Rolling, Done, Failed };

private:
    Field field_;
    Goal goal_;
    Ball ball_;
    Robot robot_;

    State state_ = State::Search;
    int tick_ = 0;

    // state Search: scan di tempat, lalu jalan zig-zag antar waypoint sambil scan
    std::vector<MathUtils::Vec2> waypoints_;
    std::size_t wpIndex_ = 0;
    int scanCount_ = 0;

    void buildWaypoints();
    void searchTick();
    void alignTick();
    void kickTick();
    void rollTick();

public:
    Simulator(const Field& field, const Goal& goal, const Ball& ball, const Robot& robot);

    // maju satu tick (1 detik)
    void tick();
    bool finished() const;

    // jalankan sampai selesai atau maxTicks. renderEvery = tampilkan lapangan tiap N tick
    // (lapangan juga selalu ditampilkan saat awal, saat ganti state, dan saat selesai)
    void run(int maxTicks = 500, int renderEvery = 1, std::ostream& os = std::cout);

    void render(std::ostream& os = std::cout) const;

    int getTick() const;
    double getElapsed() const;  // detik
    State getState() const;
    bool scored() const;
    const Field& getField() const;
    const Ball& getBall() const;
    const Robot& getRobot() const;

    static const char* stateName(State s);
};