//
// Created by muham on 15.07.2026.
//

#include "tlx/UTF-8.hpp"
#if _WIN32
    #include <windows.h>
#endif //#if _WIN32

using namespace tlx::utf8;

std::vector<char32_t> Converter::decode(const std::string &str) {
    std::vector<char32_t> result;
    size_t i = 0;
    while (i < str.length()) {
        const unsigned char cp = str[i];
        size_t bytes = 1;
        char32_t code_point = 0;

        if (cp <= 0x7F) {
            code_point = cp;
        } else if ((cp & 0xE0) == 0xC0) {
            code_point = cp & 0x1F; bytes = 2;
        } else if ((cp & 0xF0) == 0xE0) {
            code_point = cp & 0x0F; bytes = 3;
        } else if ((cp & 0xF8) == 0xF0) {
            code_point = cp & 0x07; bytes = 4;
        } else {
            //code_point = 0xFFFD;
            i++;
            continue;
        }

        if (i + bytes <= str.length()) {
            for (size_t j = 1; j < bytes; ++j) {
                code_point = (code_point << 6) | (static_cast<unsigned char>(str[i + j]) & 0x3F);
            }
            i += bytes;
        } else {
            code_point = 0xFFFD;
            i = str.length();
        }
        result.push_back(code_point);
    }
    return result;
}

std::string Converter::encode(const char32_t cp) {
    std::string result;
    if (cp <= 0x7F) {
        result.push_back(static_cast<char>(cp));
    } else if (cp <= 0x7FF) {
        result.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
        result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0xFFFF) {
        result.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
        result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0x10FFFF) {
        result.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
        result.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        return encode(0xFFFD);
    }
    return result;
}

bool tlx::utf8::IsLeadByte(const char x) {
    const auto c = static_cast<unsigned char>(x);
    return c <= 0x7F || (c & 0xC0) == 0xC0;
}

std::size_t tlx::utf8::length(const std::string &str) {
    size_t output = 0;
    for (const auto item : str) {
        if (IsLeadByte(item)) {
            output++;
        }
    }
    return output;
}

std::string tlx::utf8::substr(const std::string &str, const size_t start_char, const size_t num_chars) {
    size_t byte_start = std::string::npos;
    size_t byte_end = str.length();
    size_t char_count = 0;

    for (size_t i = 0; i < str.length(); ++i) {
        if (IsLeadByte(str[i])) {
            if (char_count == start_char) {
                byte_start = i;
            }
            if (char_count == start_char + num_chars) {
                byte_end = i;
                break;
            }
            char_count++;
        }
    }

    if (byte_start == std::string::npos) {
        return "";
    }
    return str.substr(byte_start, byte_end - byte_start);
}

std::string tlx::utf8::reverse(const std::string &str) {
    auto points = Converter::decode(str);
    std::string result;
    for (auto it = points.rbegin(); it != points.rend(); ++it) {
        result += Converter::encode(*it);
    }
    return result;
}

void tlx::utf8::SetConsoleTo() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif //#ifdef _WIN32
}