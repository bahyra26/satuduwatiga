#include <iostream>
#include "Field.h"
#include "Goal.h"

void render(const Field& f, const Goal& g) {
    for (int r = 0; r < f.getRows(); ++r) {
        for (int c = 0; c < f.getCols(); ++c) {
            std::cout << (g.isGoalCell(c, r, f) ? '#' : '.');
            if (c < f.getCols() - 1) std::cout << ' ';
        }
        std::cout << "\n";
    }
}

int main() {
    Field f;
    Goal g;
    render(f, g);
}