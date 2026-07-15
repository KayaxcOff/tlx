//
// Created by muham on 15.07.2026.
//

#include "tlx/memory.hpp"
#include <new>

std::uintptr_t tlx::alignUp(const std::uintptr_t value, const std::size_t alignment) noexcept {
    return (value + alignment - 1) & ~(alignment - 1);
}

std::uintptr_t tlx::alignDown(const std::uintptr_t value, const std::size_t alignment) {
    return value & ~(alignment - 1);
}

void *tlx::malloc(const std::size_t bytes, std::size_t alignment) {
    return operator new(bytes, static_cast<std::align_val_t>(alignment), std::nothrow);
}

void tlx::free(void *ptr, std::size_t alignment) noexcept {
    operator delete(ptr, static_cast<std::align_val_t>(alignment));
}