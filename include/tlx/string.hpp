//
// Created by muham on 16.07.2026.
//

#ifndef TLX_STRING_HPP
#define TLX_STRING_HPP

#include <tlx/macros.hpp>
#include <tlx/utility.hpp>
#include <format>
#include <string>

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
     * @brief Non-owning view over a character string (similar to `std::string_view`).
     *
     * Lightweight string view that does not take ownership of the data.
     */
    class vstring {
    public:
        /**
         * @brief Constructs an empty string view.
         */
        TLX_HD vstring() {
            this->m_data = nullptr;
            this->m_size = 0;
        }
        /**
         * @brief Constructs a string view from a character buffer.
         * @param data Pointer to the character data.
         */
        TLX_HD vstring(char* data) {
            this->m_data = data;
            this->m_size = c_str_length(data);
        }
        /**
         * @brief Constructs a string view from a character buffer.
         */
        TLX_HD vstring(const char* data) {
            this->m_data = const_cast<char*>(data);
            this->m_size = c_str_length(data);
        }
        TLX_HD vstring(char* data, const std::size_t size) {
            this->m_data = data;
            this->m_size = size;
        }
        TLX_HD vstring(const vstring& other) {
            this->m_data = other.m_data;
            this->m_size = other.m_size;
        }
        TLX_HD vstring(vstring&& other) noexcept {
            this->m_data = other.m_data;
            this->m_size = other.m_size;
            other.m_data = nullptr;
            other.m_size = 0;
        }
        TLX_HD ~vstring() = default;

        /**
         * @brief Returns a pointer to the underlying character data.
         */
        [[nodiscard]]
        TLX_HD char* data() {
            return this->m_data;
        }
        /**
         * @brief Returns a const pointer to the underlying character data.
         */
        [[nodiscard]]
        TLX_HD const char* data() const {
            return this->m_data;
        }
        /**
         * @brief Returns the length of the string.
         */
        [[nodiscard]]
        TLX_HD std::size_t size() const {
            return this->m_size;
        }
        /**
         * @brief Checks whether the string view is empty.
         */
        [[nodiscard]]
        TLX_HD bool empty() const {
            return this->m_size == 0;
        }
        [[nodiscard]]
        TLX_HD vstring substr(const std::size_t offset, const std::size_t size) const {
            return vstring(this->m_data + offset, size);
        }
        [[nodiscard]]
        TLX_HD char* begin() {
            return this->m_data;
        }
        [[nodiscard]]
        TLX_HD const char* begin() const {
            return this->m_data;
        }
        [[nodiscard]]
        TLX_HD char* end() {
            return this->m_data + this->m_size;
        }
        [[nodiscard]]
        TLX_HD const char* end() const {
            return this->m_data + this->m_size;
        }

        TLX_HD char& operator[](const std::size_t index) {
            //detail::exitIf(index >= this->m_size, "index out of bounds");
            return this->m_data[index];
        }
        TLX_HD const char& operator[](const std::size_t index) const {
            //detail::exitIf(index >= this->m_size, "index out of bounds");
            return this->m_data[index];
        }
        TLX_HD bool operator==(const vstring& other) const {
            if (this->m_size != other.m_size) {
                return false;
            }
            for (std::size_t i = 0; i < this->m_size; ++i) {
                if (this->m_data[i] != other.m_data[i]) {
                    return false;
                }
            }
            return true;
        }
        TLX_HD bool operator!=(const vstring& other) const {
            return !(*this == other);
        }
        TLX_HD vstring& operator=(const vstring& other) {
            if (this != &other) {
                this->m_data = other.m_data;
                this->m_size = other.m_size;
            }
            return *this;
        }
        TLX_HD vstring& operator=(vstring&& other) noexcept {
            if (this != &other) {
                this->m_data = other.m_data;
                this->m_size = other.m_size;
                other.m_size = 0;
            }
            return *this;
        }
    private:
        char* m_data;
        std::size_t m_size;

        [[nodiscard]]
        TLX_HD TLX_INLINE static std::size_t c_str_length(const char* s) {
            std::size_t len = 0;
            while (s[len] != '\0') {
                ++len;
            }
            return len;
        }
    };

    std::ostream& operator<<(std::ostream& os, const vstring& rhs);
} //namespace tlx

#endif //TLX_STRING_HPP