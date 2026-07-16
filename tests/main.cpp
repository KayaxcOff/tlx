//
// Created by muham on 15.07.2026.
//

#include <tlx/string.hpp>
#include <iostream>

int main() {
    const tlx::vstring x1("Hello World", 11);

    std::cout << "Value of vstring: " << x1 << std::endl;
    std::cout << "Size of vstring: " << x1.size() << std::endl;
    std::cout << "Sizeof of vstring: " << sizeof(x1) << " bytes" << std::endl;

    return 0;
}