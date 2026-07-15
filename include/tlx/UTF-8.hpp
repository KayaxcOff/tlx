//
// Created by muham on 15.07.2026.
//

#ifndef TLX_UTF_8_HPP
#define TLX_UTF_8_HPP

#include <string>
#include <vector>

namespace tlx::utf8 {
    /**
     * @brief Utility struct for UTF-8 encoding and decoding operations.
     */
    struct Converter {
        /**
         * @brief Decodes a UTF-8 string into a sequence of Unicode code points.
         *
         * @param str UTF-8 encoded input string.
         * @return Vector containing the decoded `char32_t` code points.
         */
        [[nodiscard]]
        static std::vector<char32_t> decode(const std::string& str);
        /**
         * @brief Encodes a single Unicode code point into UTF-8.
         *
         * @param cp The Unicode code point to encode.
         * @return UTF-8 encoded string for the given code point.
         */
        [[nodiscard]]
        static std::string encode(char32_t cp);
    };

    /**
     * @brief Checks whether a byte is the leading byte of a UTF-8 character.
     *
     * @param x The byte to check.
     * @return `true` if the byte is a lead byte (ASCII or start of a multi-byte sequence).
     */
    [[nodiscard]]
    bool IsLeadByte(char x);
    /**
     * @brief Returns the number of Unicode characters (code points) in a UTF-8 string.
     *
     * @param str The UTF-8 encoded string.
     * @return Number of Unicode characters.
     */
    [[nodiscard]]
    std::size_t length(const std::string& str);
    /**
     * @brief Extracts a substring by character count (not byte count).
     *
     * @param str         Source UTF-8 string.
     * @param start_char  Starting character index (0-based).
     * @param num_chars   Number of characters to extract.
     * @return Substring containing the requested characters.
     */
    [[nodiscard]]
    std::string substr(const std::string& str, size_t start_char, size_t num_chars);
    /**
     * @brief Reverses the order of Unicode characters in a UTF-8 string.
     *
     * @param str The UTF-8 string to reverse.
     * @return Reversed UTF-8 string.
     */
    [[nodiscard]]
    std::string reverse(const std::string& str);
    /**
     * @brief Sets the console output code page to UTF-8 (Windows only).
     *
     * This function has no effect on non-Windows platforms.
     */
    void SetConsoleTo();
} //namespace tlx::utf8

#endif //TLX_UTF_8_HPP