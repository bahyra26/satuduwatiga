#include <iostream>
#include "Field.h"

void render(const Field& f) {
    for (int r = 0; r < f.getRows(); ++r) {
        for (int c = 0; c < f.getCols(); ++c) {
            std::cout << '.';
            if (c < f.getCols() - 1) std::cout << ' ';
        }
        std::cout << "\n";
    }
}

int main() {
    Field f;
    render(f);
}