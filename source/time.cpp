//
// Created by muham on 15.07.2026.
//

#include "tlx/time.hpp"

using namespace tlx;

StopWatch::StopWatch() {
    this->m_start = std::chrono::steady_clock::now();
}

StopWatch::~StopWatch() = default;

void StopWatch::restart() noexcept {
    this->m_start = std::chrono::steady_clock::now();
}