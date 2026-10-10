#include <exception>
#include <iostream>
#include "Field.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"
#include "Simulator.h"

// minta titik (x, y) sampai valid: angka, di dalam lapangan
static MathUtils::Vec2 askPoint(const char* name, const Field& field) {
    while (true) {
        double x = 0.0, y = 0.0;
        std::cout << "Koordinat " << name << " (x y): ";
        if (!(std::cin >> x >> y)) {
            if (std::cin.eof()) throw std::runtime_error("Input berakhir");
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  Input harus berupa dua angka.\n";
            continue;
        }
        MathUtils::Vec2 p{x, y};
        if (!field.isInside(p)) {
            std::cout << "  Di luar lapangan (x: -4.5..4.5, y: -3..3).\n";
            continue;
        }
        return p;
    }
}

int main() {
    try {
        Field field;
        Goal goal;

        MathUtils::Vec2 robotPos = askPoint("robot", field);
        MathUtils::Vec2 ballPos;
        while (true) {
            ballPos = askPoint("bola", field);
            if (field.cellOf(ballPos) != field.cellOf(robotPos)) break;
            std::cout << "  Bola tidak boleh satu petak dengan robot.\n";
        }

        Ball ball(ballPos, field);
        Robot robot(robotPos, 90, field, goal);

        Simulator sim(field, goal, ball, robot);
        sim.run(500, 1);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
}