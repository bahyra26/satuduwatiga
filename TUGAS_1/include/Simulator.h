#pragma once
#include <cstddef>
#include <iostream>
#include <vector>
#include "MathUtils.h"
#include "Field.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"
#include "Striker.h"

class Simulator {
public:
    static constexpr double TICK_SECONDS = 1.0;

    enum class State { Search, Align, Kick, Rolling, Done, Failed };

private:
    Field field_;
    Goal goal_;
    Ball ball_;
    Striker robot_;

    State state_ = State::Search;
    int tick_ = 0;

    std::vector<MathUtils::Vec2> waypoints_;
    std::size_t wpIndex_ = 0;
    int scanCount_ = 0;

    void buildWaypoints();
    void searchTick();
    void alignTick();
    void kickTick();
    void rollTick();
    void drawFrame(std::ostream& os) const;

public:
    Simulator(const Field& field, const Goal& goal, const Ball& ball, const Striker& robot);

    void tick();
    bool finished() const;

    void run(int maxTicks = 500, int renderEvery = 1, int delayMs = 100, std::ostream& os = std::cout);
    void render(std::ostream& os = std::cout) const;

    int getTick() const;
    double getElapsed() const;
    State getState() const;
    bool scored() const;
    const Field& getField() const;
    const Ball& getBall() const;
    const Robot& getRobot() const;

    static const char* stateName(State s);
    
    int respawns_ = 0;  // berapa kali bola direspawn
    
    void respawnBall();
    
    int getRespawns() const;
};