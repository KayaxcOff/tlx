//
// Created by muham on 16.07.2026.
//

#include "tlx/string.hpp"
#include <iostream>
#include <cstdint>

namespace tlx {
    std::ostream& operator<<(std::ostream& os, const vstring& string) {
        return os.write(string.data(), static_cast<std::int64_t>(string.size()));
    }
} //namespace tlx