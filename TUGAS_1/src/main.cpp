#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include "Field.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"

using MathUtils::Vec2;

using Cell = std::pair<int, int>;

// petak dari posisi, dijepit ke dalam lapangan (titik di garis tepi tetap tampil)
Cell cellOf(const Field& f, const Vec2& p) {
    auto [c, r] = f.toGrid(p);
    return {std::clamp(c, 0, f.getCols() - 1), std::clamp(r, 0, f.getRows() - 1)};
}

// panah sesuai hadapan robot, dibulatkan ke 4 arah
char headingChar(double heading) {
    switch (((static_cast<int>(std::lround(heading / 90.0)) % 4) + 4) % 4) {
        case 0: return '>';
        case 1: return '^';
        case 2: return '<';
        default: return 'v';
    }
}

void render(const Field& f, const Goal& g, const Ball& b, const Robot* robot = nullptr,
            const std::set<Cell>& trail = {}) {
    Cell ballCell = cellOf(f, b.getPosition());
    Cell robotCell{-1, -1};
    if (robot) robotCell = cellOf(f, robot->getPosition());

    for (int r = 0; r < f.getRows(); ++r) {
        for (int c = 0; c < f.getCols(); ++c) {
            char ch = '.';
            if (trail.count({c, r})) ch = '*';
            if (g.isGoalCell(c, r, f)) ch = '#';
            if (Cell{c, r} == ballCell) ch = 'O';
            if (Cell{c, r} == robotCell) ch = headingChar(robot->getHeading());
            std::cout << ch;
            if (c < f.getCols() - 1) std::cout << ' ';
        }
        std::cout << "\n";
    }
}

// tendang bola lalu jalankan per petak sampai berhenti, catat jejaknya
void testKick(const char* name, const Vec2& start, double angleDeg) {
    Field f;
    Goal g;
    Ball b(start, f);
    std::set<Cell> trail{cellOf(f, b.getPosition())};

    b.kick(angleDeg);
    while (b.step(f)) trail.insert(cellOf(f, b.getPosition()));

    Cell end = cellOf(f, b.getPosition());
    std::cout << "== " << name << " (sudut " << angleDeg << ") ==\n";
    render(f, g, b, nullptr, trail);
    std::cout << "berhenti di petak (" << end.first << ", " << end.second << ")  "
              << (g.isGoalCell(end.first, end.second, f) ? "GOL" : "tidak gol") << "\n\n";
}

// taruh robot di posisi + hadapan tertentu, tampilkan info arah ke gawang
void testRobot(const char* name, const Vec2& pos, double heading) {
    Field f;
    Goal g;
    Ball b({0.25, 0.25}, f);
    Robot r(pos, heading, f);

    std::cout << "== " << name << " ==\n";
    render(f, g, b, &r);
    std::cout << "posisi (" << r.getPosition().x << ", " << r.getPosition().y << ")"
              << "  hadap " << r.getHeading() << " derajat\n"
              << "jarak ke gawang " << r.goalDistance(g) << " m"
              << ", sudut ke gawang " << r.goalBearing(g)
              << ", relatif ke hadapan " << r.goalRelativeAngle(g) << "\n\n";
}

// R = robot, @ = petak yang kelihatan robot, O = bola, # = gawang
void renderVision(const Field& f, const Goal& g, const Ball& b, const Robot& robot) {
    Cell ballCell = cellOf(f, b.getPosition());
    Cell robotCell = cellOf(f, robot.getPosition());

    for (int r = 0; r < f.getRows(); ++r) {
        for (int c = 0; c < f.getCols(); ++c) {
            char ch = '.';
            if (robot.canSee(f.toWorld(c, r))) ch = '@';
            if (g.isGoalCell(c, r, f)) ch = '#';
            if (Cell{c, r} == ballCell) ch = 'O';
            if (Cell{c, r} == robotCell) ch = 'R';
            std::cout << ch;
            if (c < f.getCols() - 1) std::cout << ' ';
        }
        std::cout << "\n";
    }
}

int main() {
    try {
        Field f;
        Goal g;
        Ball b({1.25, 0.75}, f);
        Robot r({-2.25, 0.25}, 0, f, 90, 1.5);  // posisi, hadap, fov, jarak pandang

        renderVision(f, g, b, r);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}