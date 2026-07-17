//
// Created by muham on 15.07.2026.
//

#include <tlx/random.hpp>
#include <tlx/types.hpp>
#include <iostream>

int main() {
    tlx::philox4x32 rng(1);

    std::cout << rng.uniform<tlx::bfloat16>() << std::endl;

    return 0;
}