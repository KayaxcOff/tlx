//
// Created by muham on 15.07.2026.
//

#ifndef TLX_VEC_HPP
#define TLX_VEC_HPP

#include <tlx/errors.hpp>
#include <tlx/macros.hpp>
#include <tlx/memory.hpp>
#include <tlx/utility.hpp>
#include <cstddef>
#include <initializer_list>
#include <vector>

namespace tlx {
    /**
     * @brief Fixed-capacity stack-allocated vector.
     *
     * A lightweight, exception-safe vector-like container with compile-time fixed capacity.
     * All elements are stored in-place (no heap allocation).
     *
     * @tparam T Element type.
     * @tparam N Maximum capacity of the vector.
     */
    template<typename T, std::size_t N>
    class vec {
    public:
        /**
         * @brief Constructs an empty vector.
         */
        TLX_HD vec() : m_raw_data{} {
            this->m_size = 0;
        }

        /**
         * @brief Constructs from an initializer list.
         */
        TLX_HOST vec(const std::initializer_list<T>& list) : m_raw_data{} {
            detail::exitIf(list.size() > N, "Initializer list exceeds vector capacity");
            this->m_size = list.size();
            std::size_t i = 0;
            for (const auto& item : list) {
                ::tlx::construct(data() + i, item);
                i++;
            }
        }

        /**
         * @brief Constructs from a `std::vector`.
         */
        TLX_HOST explicit vec(const std::vector<T>& vector) : m_raw_data{} {
            detail::exitIf(vector.size() > N, "Vector size exceeds static capacity");
            this->m_size = vector.size();
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::construct(data() + i, vector[i]);
            }
        }
        TLX_HD vec(const vec& other) : m_raw_data{}, m_size(other.m_size) {
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::construct(data() + i, other[i]);
            }
        }
        TLX_HD vec(vec&& other) noexcept : m_raw_data{}, m_size(other.m_size) {
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::construct(data() + i, ::tlx::move(other[i]));
                ::tlx::destroy(other.data() + i);
            }
            other.m_size = 0;
        }

        TLX_HD ~vec() {
            clear();
        }
        /**
         * @brief Returns a pointer to the underlying raw data.
         */
        [[nodiscard]]
        TLX_HD T* data() noexcept {
            return reinterpret_cast<T*>(this->m_raw_data);
        }
        /**
         * @brief Returns a const pointer to the underlying raw data.
         */
        [[nodiscard]]
        TLX_HD const T* data() const noexcept {
            return reinterpret_cast<const T*>(this->m_raw_data);
        }
        /**
         * @brief Returns the current number of elements.
         */
        [[nodiscard]]
        TLX_HD std::size_t size() const  noexcept {
            return this->m_size;
        }
        /**
         * @brief Returns the maximum capacity of the vector.
         */
        [[nodiscard]]
        TLX_HD static std::size_t capacity() noexcept {
            return N;
        }
        /**
         * @brief Checks whether the vector is empty.
         */
        [[nodiscard]]
        TLX_HD bool empty() const noexcept {
            return this->m_size == 0;
        }
        /**
         * @brief Constructs an element in-place at the end of the vector.
         *
         * @tparam Args Constructor argument types.
         * @param args Arguments to forward to the constructor of `T`.
         * @return Reference to the newly constructed element.
         */
        template<typename... Args>
        TLX_HD T& emplace_back(Args&&... args) {
            detail::exitIf(this->m_size >= N, "Vector size exceeds static capacity");
            T* allocated_ptr = ::tlx::construct(data() + this->m_size, ::tlx::forward<Args>(args)...);
            ++this->m_size;
            return *allocated_ptr;
        }
        /**
         * @brief Removes the last element.
         */
        TLX_HD void push_back(const T& value) {
            this->emplace_back(value);
        }
        /**
         * @brief Resizes the vector to the given size.
         *
         * Destroys excess elements or default-constructs new ones as needed.
         */
        TLX_HD void push_back(T&& value) {
            this->emplace_back(::tlx::move(value));
        }
        TLX_HD void pop_back() {
            detail::exitIf(empty(), "Cannot pop from an empty vector");
            --this->m_size;
            ::tlx::destroy(data() + this->m_size);
        }
        /**
         * @brief Resizes the vector to the given size.
         *
         * Destroys excess elements or default-constructs new ones as needed.
         */
        TLX_HD void resize(const std::size_t new_size) {
            detail::exitIf(new_size > N, "New size exceeds static capacity");
            if (new_size < this->m_size) {
                for (std::size_t i = new_size; i < this->m_size; ++i) {
                    ::tlx::destroy(data() + i);
                }
            } else if (new_size > this->m_size) {
                for (std::size_t i = this->m_size; i < new_size; ++i) {
                    ::tlx::construct(data() + i);
                }
            }

            this->m_size = new_size;
        }
        /**
         * @brief Destroys all elements and sets size to zero.
         */
        TLX_HD void clear() noexcept {
            for (std::size_t i = 0; i < this->m_size; ++i) {
                ::tlx::destroy(data() + i);
            }
            this->m_size = 0;
        }

        [[nodiscard]]
        TLX_HD T& operator[](std::size_t index) {
            detail::exitIf(index >= this->m_size, "Out of bounds access");
            return data()[index];
        }

        [[nodiscard]]
        TLX_HD const T& operator[](std::size_t index) const {
            detail::exitIf(index >= this->m_size, "Out of bounds access");
            return data()[index];
        }

        [[nodiscard]]
        TLX_HD T* begin() noexcept {
            return reinterpret_cast<T*>(this->m_raw_data);
        }
        [[nodiscard]]
        TLX_HD T* end() noexcept {
            return data() + this->m_size;
        }
        [[nodiscard]]
        TLX_HD const T* begin() const noexcept {
            return reinterpret_cast<const T*>(this->m_raw_data);
        }
        [[nodiscard]]
        TLX_HD const T* end() const noexcept {
            return data() + this->m_size;
        }
        [[nodiscard]]
        TLX_HD const T* cbegin() const noexcept {
            return reinterpret_cast<const T*>(this->m_raw_data);
        }
        [[nodiscard]]
        TLX_HD const T* cend() const noexcept {
            return data() + this->m_size;
        }

        operator std::vector<T>() {
            return {begin(), end()};
        }

        TLX_HD vec& operator=(const vec& other) {
            if (this != &other) {
                clear();
                this->m_size = other.m_size;
                for (std::size_t i = 0; i < this->m_size; i++) {
                    ::tlx::construct(data() + i, other[i]);
                }
            }
            return *this;
        }

        TLX_HD vec& operator=(vec&& other) noexcept {
            if (this != &other) {
                clear();
                this->m_size = other.m_size;
                for (std::size_t i = 0; i < m_size; i++) {
                    ::tlx::construct(data() + i, ::tlx::move(other[i]));
                    ::tlx::destroy(other.data() + i);
                }
            }
            other.m_size = 0;
            return *this;
        }

    private:
        alignas(T) std::byte m_raw_data[N * sizeof(T)];
        std::size_t m_size = 0;
    };
} //namespace tlx

#endif //TLX_VEC_HPP