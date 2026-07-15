//
// Created by muham on 15.07.2026.
//

#include "tlx/buffer.hpp"
#include <tlx/utility.hpp>

using namespace tlx;

BufferView::BufferView() {
    this->m_data = nullptr;
    this->m_size = 0;
}

BufferView::BufferView(std::byte *data, const std::size_t size) {
    this->m_data = data;
    this->m_size = size;
}

BufferView::BufferView(const std::byte *data, const std::size_t size) : m_data(nullptr) {
    this->m_data = const_cast<std::byte*>(data);
    this->m_size = size;
}

BufferView::BufferView(const BufferView &other) {
    this->m_data = other.m_data;
    this->m_size = other.m_size;
}

BufferView::BufferView(BufferView &&other) noexcept {
    this->m_data = ::tlx::move(other.m_data);
    this->m_size = other.m_size;
}

BufferView::~BufferView() = default;

std::size_t BufferView::size() const {
    return this->m_size;
}

BufferView &BufferView::operator=(const BufferView &other) {
    if (this == &other) {
        return *this;
    }
    this->m_data = other.m_data;
    this->m_size = other.m_size;
    return *this;
}

BufferView &BufferView::operator=(BufferView &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    this->m_data = ::tlx::move(other.m_data);
    this->m_size = other.m_size;
    return *this;
}