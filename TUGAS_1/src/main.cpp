#include <exception>
#include <iostream>
#include "Field.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"
#include "Simulator.h"

int main() {
    try {
        Field field;
        Goal goal;
        Ball ball({0, 3.0}, field);
        Robot robot({-3.75, -3.0}, 90, field, goal);  // robot diberi tahu lokasi gawang

        Simulator sim(field, goal, ball, robot);
        sim.run(500, 5);  // maks 500 tick (detik), tampilkan lapangan tiap 5 tick
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
}