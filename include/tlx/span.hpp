//
// Created by muham on 15.07.2026.
//

#ifndef TLX_SPAN_HPP
#define TLX_SPAN_HPP

#include <tlx/buffer.hpp>
#include <tlx/errors.hpp>
#include <tlx/utility.hpp>

namespace tlx {
    /**
     * @brief A non-owning view over a contiguous sequence of objects of type `T`.
     *
     * This class provides a lightweight, read/write (or read-only) view over an array
     * or buffer, similar to `std::span<T>` in C++20.
     *
     * @tparam T Type of the elements in the span.
     */
    template<typename T>
    class Span {
    public:
        /**
         * @brief Constructs an empty span.
         */
        Span() {
            this->m_buf = BufferView();
        }
        /**
         * @brief Constructs a span from a pointer and size.
         * @param value Pointer to the first element.
         * @param size  Number of elements.
         */
        Span(const T* value, std::size_t size) {
            this->m_buf = BufferView(value, size);
        }
        Span(const Span& other) {
            this->m_buf = other.m_buf;
        }
        Span(Span&& other) noexcept {
            this->m_buf = ::tlx::move(other.m_buf);
        }
        ~Span() = default;

        /**
         * @brief Returns a pointer to the first element.
         */
        [[nodiscard]]
        T* data() {
            return this->m_buf.as<T>();
        }
        /**
         * @brief Returns a const pointer to the first element.
         */
        [[nodiscard]]
        const T* data() const {
            return this->m_buf.as<T>();
        }
        /**
         * @brief Returns the number of elements in the span.
         */
        [[nodiscard]]
        std::size_t size() const {
            return this->m_buf.size();
        }
        /**
         * @brief Checks whether the span is empty.
         */
        [[nodiscard]]
        bool empty() const {
            return this->m_buf.size() == 0;
        }

        /**
         * @brief Returns an iterator to the beginning.
         */
        [[nodiscard]]
        T* begin() {
            return this->m_buf.as<T>();
        }
        /**
         * @brief Returns an iterator to the end.
         */
        [[nodiscard]]
        T* end() {
            return this->m_buf.as<T>() + this->m_buf.size();
        }
        /**
         * @brief Returns a const iterator to the beginning.
         */
        [[nodiscard]]
        const T* cbegin() const {
            return this->m_buf.as<T>();
        }
        /**
         * @brief Returns a const iterator to the end.
         */
        [[nodiscard]]
        const T* cend() const {
            return this->m_buf.as<T>() + this->m_buf.size();
        }

        /**
         * @brief Subscript operator
         * @param index Index of the element.
         */
        [[nodiscard]]
        T& operator[](std::size_t index) {
            detail::exitIf(index > this->m_buf.size(), "index out of range");
            return this->m_buf.as<T>()[index];
        }
        /**
         * @brief Const subscript operator
         * @param index Index of the element.
         */
        [[nodiscard]]
        const T& operator[](std::size_t index) const {
            detail::exitIf(index > this->m_buf.size(), "index out of range");
            return this->m_buf.as<T>()[index];
        }

        Span& operator=(const Span& other) {
            if (this != &other) {
                this->m_buf = other.m_buf;
            }
            return *this;
        }
        Span& operator=(Span&& other) noexcept {
            if (this != &other) {
                this->m_buf = ::tlx::move(other.m_buf);
            }
            return *this;
        }
    private:
        BufferView m_buf;
    };
} //namespace tlx

#endif //TLX_SPAN_HPP