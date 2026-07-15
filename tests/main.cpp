//
// Created by muham on 15.07.2026.
//

#include <tlx/vec.hpp>
#include <tlx/types.hpp>

int main() {
    tlx::vec<tlx::half, 3> x1 = {
        1, 2, 3
    };

    x1.push_back(1);

    return 0;
}