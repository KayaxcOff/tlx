//
// Created by muham on 15.07.2026.
//

#include "tlx/types.hpp"
#include <iostream>

namespace tlx {
    std::ostream& operator<<(std::ostream &os, const bfloat16 &rhs) {
        os << static_cast<float>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const half& rhs) {
        os << static_cast<float>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream &os, const qint8 &rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const qint16& rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const quint8& rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const quint16& rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
} //namespace tlx