#include <iostream>
#include "Field.h"

int main() {
    Field f;
    for (int r = 0; r < f.getRows(); ++r) {
        for (int c = 0; c < f.getCols(); ++c) {
            std::cout << (f.isGoalCell(c, r) ? '#' : '.');
            if (c < f.getCols() - 1) std::cout << ' ';
        }
        std::cout << "\n";
    }
    auto a = f.toGrid({0, 0});
    auto b = f.toGrid({4.5, 1.5});
    std::cout << a.first << "," << a.second << "\n";  // 9,6
    std::cout << b.first << "," << b.second << "\n";  // 18,3
}