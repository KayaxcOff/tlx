//
// Created by muham on 15.07.2026.
//

#include "tlx/errors.hpp"
#include <tlx/utility.hpp>
#include <cstdio>
#ifdef _WIN32
    #include <windows.h>
#endif //#ifdef _WIN32

using namespace tlx;

namespace {
    void enable_virtual_terminal_processing() {
#ifdef _WIN32
        static bool enabled = false;
        if (!enabled) {
            HANDLE hOut = GetStdHandle(STD_ERROR_HANDLE);
            if (hOut != INVALID_HANDLE_VALUE) {
                DWORD dwMode = 0;
                if (GetConsoleMode(hOut, &dwMode)) {
                    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(hOut, dwMode);
                }
            }
            enabled = true;
        }
#endif //#ifdef _WIN32
    }

    constexpr const char* trim_path(const char* path) {
        const char* p = path;

        while (*p) {
            if (p[0] == 't' && p[1] == 'l' && p[2] == 'x' && (p[3] == '/' || p[3] == '\\')) {
                return p;
            }
            ++p;
        }

        return path;
    }
} //unnamed namespace

Exception::Exception(const char *message) {
    this->m_message = message;
}

Exception::Exception(std::string message) {
    this->m_message = ::tlx::move(message);
}

Exception::~Exception() = default;

char const *Exception::what() const {
    return this->m_message.c_str();
}

void detail::exitIf(const bool condition, const char *message, const std::source_location &loc) {
    if (condition) {
        enable_virtual_terminal_processing();
        std::fprintf(stderr, "\033[1;31m[%s:%u]\033[0m : %s\n", trim_path(loc.file_name()), loc.line(), message);
        std::exit(TLX_ERR_EXIT);
    }
}

void detail::warnIf(const bool condition, const char *message, const std::source_location &loc) {
    if (condition) {
        enable_virtual_terminal_processing();
        std::fprintf(stderr, "\033[1;33m[%s:%u]\033[0m : %s\n", trim_path(loc.file_name()), loc.line(), message);
    }
}

Status::Status() {
    this->m_code = ErrorCode::kDefault;
}

Status::Status(const ErrorCode code) {
    this->m_code = code;
}

Status::Status(const ErrorCode code, std::string message) {
    this->m_code = code;
    this->m_message = ::tlx::move(message);
}

Status::~Status() = default;

bool Status::success() const {
    return this->m_code == ErrorCode::kSuccess;
}

ErrorCode Status::code() const {
    return this->m_code;
}

const std::string &Status::message() {
    return this->m_message;
}

Status tlx::Success() {
    return {};
}

Status tlx::Empty(const std::string &message) {
    return {ErrorCode::kEmpty, message};
}

Status tlx::Null(const std::string &message) {
    return {ErrorCode::kEmpty, message};
}

Status tlx::InvalidArgument(const std::string &message) {
    return {ErrorCode::kInvalidParameter, message};
}

Status tlx::OutOfMemory(const std::string &message) {
    return {ErrorCode::kOutOfMemory, message};
}

Status tlx::FileNotFound(const std::string &message) {
    return {ErrorCode::kFileNotFound, message};
}

Status tlx::IO(const std::string &message) {
    return {ErrorCode::IOError, message};
}