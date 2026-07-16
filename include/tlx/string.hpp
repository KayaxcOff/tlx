//
// Created by muham on 16.07.2026.
//

#ifndef TLX_STRING_HPP
#define TLX_STRING_HPP

#include <tlx/macros.hpp>
#include <tlx/utility.hpp>
#include <format>
#include <string>
#include <string_view>

namespace tlx {
    /**
     * @brief Type-safe string formatting using `std::format`.
     *
     * This is a thin wrapper around `std::format` for convenience.
     *
     * @tparam Args Argument types for formatting.
     * @param fmt Format string.
     * @param args Arguments to format.
     * @return Formatted string.
     */
    template<typename ... Args>
    [[nodiscard]]
    std::string format(std::format_string<Args...> fmt, Args ... args) {
        return std::format(fmt, ::tlx::forward<Args>(args)...);
    }

    /**
     * @brief Non-owning view over a constant character string. As simple, view-string
     *
     * A lightweight, non-owning string view similar to `std::string_view`,
     * but with additional constructors for convenience and CUDA/host compatibility.
     */
    class vstring {
    public:
        /**
         * @brief Constructs an empty string view.
         */
        TLX_HD constexpr vstring() noexcept {
            this->m_data = nullptr;
            this->m_size = 0;
        }
        /**
         * @brief Constructs a string view from a character pointer and length.
         *
         * @param data Pointer to the character data.
         * @param size Length of the string in bytes (excluding null terminator).
         */
        TLX_HD constexpr vstring(const char* data, const std::size_t size) noexcept {
            this->m_data = data;
            this->m_size = size;
        }
        /**
         * @brief Constructs a string view from a string literal.
         */
        template<std::size_t N>
        TLX_HD constexpr vstring(const char (&literal)[N]) noexcept {
            this->m_data = literal;
            this->m_size = N - 1;
        }
        /**
         * @brief Constructs a string view from a `std::string`.
         */
        TLX_HOST constexpr vstring(const std::string& string) noexcept {
            this->m_data = string.data();
            this->m_size = string.size();
        }
        TLX_HD constexpr vstring(const vstring&) = default;
        TLX_HD constexpr vstring(vstring&&) noexcept = default;
        TLX_HD ~vstring() = default;

        /**
         * @brief Returns a pointer to the underlying character data.
         */
        [[nodiscard]]
        TLX_HD constexpr const char* c() const noexcept {
            return this->m_data;
        }
        /**
         * @brief Checks whether the string view is empty.
         */
        [[nodiscard]]
        TLX_HD constexpr std::size_t size() const noexcept {
            return this->m_size;
        }
        [[nodiscard]]
        TLX_HD constexpr bool empty() const noexcept {
            return this->m_size == 0;
        }
        /**
         * @brief Returns a substring view.
         *
         * @param offset Starting position.
         * @param count  Number of characters to take.
         * @return A new `vstring` representing the substring.
         */
        [[nodiscard]]
        TLX_HD constexpr vstring substr(const std::size_t offset, const std::size_t count) const {
            if (offset > this->m_size) {
                return {};
            }
            std::size_t actual_count = (offset + count > this->m_size) ? (this->m_size - offset) : count;
            return {this->m_data + offset, actual_count};
        }
        [[nodiscard]]
        TLX_HD constexpr const char* begin() const noexcept {
            return this->m_data;
        }
        [[nodiscard]]
        TLX_HD constexpr const char* end() const noexcept {
            return this->m_data + this->m_size;
        }

        /**
         * @brief Implicit conversion to `std::string_view`.
         */
        TLX_HOST constexpr operator std::string_view() const noexcept {
            return {this->m_data, this->m_size};
        }

        [[nodiscard]]
        TLX_HD constexpr const char& operator[](const std::size_t index) const noexcept {
            return this->m_data[index];
        }
        TLX_HD constexpr bool operator==(const vstring& other) const noexcept {
            if (this->m_size != other.m_size) {
                return false;
            }
            if (this->m_data == other.m_data) {
                return true;
            }

            for (std::size_t i = 0; i < this->m_size; ++i) {
                if (this->m_data[i] != other.m_data[i]) {
                    return false;
                }
            }
            return true;
        }
        TLX_HD constexpr bool operator!=(const vstring& other) const noexcept {
            return !(*this == other);
        }
    private:
        const char* m_data;
        std::size_t m_size;
    };

    std::ostream& operator<<(std::ostream& os, const vstring& string);
} //namespace tlx

#endif //TLX_STRING_HPP