#include <iostream>
#include "Field.h"
#include "Goal.h"
#include "Ball.h"

void render(const Field& f, const Goal& g, const Ball& b) {
    auto [bc, br] = f.toGrid(b.getPosition());
    for (int r = 0; r < f.getRows(); ++r) {
        for (int c = 0; c < f.getCols(); ++c) {
            char ch = '.';
            if (g.isGoalCell(c, r, f)) ch = '#';
            if (c == bc && r == br) ch = 'O';
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
    Ball b({1.25, 0.25}, f); 
    render(f, g, b);
} catch (const std ::exception& e){
     std::cout << "Error: " << e.what() << "\n";
}
}