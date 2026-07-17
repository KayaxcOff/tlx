//
// Created by muham on 15.07.2026.
//

#include <tlx/random.hpp>
#include <tlx/types.hpp>
#include <iostream>

int main() {
    tlx::Alea x1(2);

    std::cout << "--- bfloat16 ---" << std::endl;
    for(int i = 0; i < 5; ++i) {
        std::cout << x1.next<tlx::bfloat16>() << std::endl;
    }

    tlx::Alea x2(2);
    std::cout << "\n--- half  ---" << std::endl;
    for(int i = 0; i < 5; ++i) {
        std::cout << x2.next<tlx::half>() << std::endl;
    }

    tlx::Alea x3(999);
    std::cout << "\n--- bfloat16, seed 999 ---" << std::endl;
    std::cout << x3.next<tlx::bfloat16>() << std::endl;

    return 0;
}