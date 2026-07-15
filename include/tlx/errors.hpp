//
// Created by muham on 15.07.2026.
//

#ifndef TLX_ERRORS_HPP
#define TLX_ERRORS_HPP

#include <tlx/macros.hpp>
#include <exception>
#include <source_location>
#include <string>

namespace tlx {
    /**
     * @brief Base exception class for the TLX library.
     *
     * This class inherits from `std::exception` and provides a simple way to throw
     * exceptions with custom messages within the TLX codebase.
     */
    class Exception : public std::exception {
    public:
        /**
         * @brief Constructs an exception with a C-style string message.
         * @param message Error message (null-terminated).
         */
        explicit Exception(const char* message);
        /**
         * @brief Constructs an exception with a `std::string` message.
         * @param message Error message.
         */
        explicit Exception(std::string message);
        ~Exception() override;

        /**
         * @brief Returns the explanatory string.
         * @return C-style string containing the exception message.
         */
        [[nodiscard]]
        char const *what() const override;
    private:
        std::string m_message;
    };

    namespace detail {
        TLX_HD void exitIf(bool condition, const char* message, const std::source_location& loc = std::source_location::current());
        TLX_HD void warnIf(bool condition, const char* message, const std::source_location& loc = std::source_location::current());
    } //namespace detail

    enum class ErrorCode : std::uint8_t {
        kSuccess = 0,
        kEmpty = 1,
        kNullPointer = 2,
        kOutOfMemory = 3,
        kInvalidParameter = 4,
        kFileNotFound = 5,
        IOError = 6,
        kDefault = 7
    };

    /**
     * @brief Represents the result of an operation (success or error).
     *
     * This class is similar to `std::expected` or custom result types. It holds
     * an error code and an optional descriptive message.
     */
    class Status {
    public:
        /**
         * @brief Constructs a status with default error code (`kDefault`).
         */
        Status();
        /**
         * @brief Constructs a status with the given error code.
         * @param code The error code.
         */
        explicit Status(ErrorCode code);
        /**
         * @brief Constructs a status with error code and message.
         * @param code    The error code.
         * @param message Descriptive error message.
         */
        Status(ErrorCode code, std::string message);
        ~Status();

        /**
         * @brief Checks whether the operation was successful.
         * @return `true` if the status code is `kSuccess`.
         */
        [[nodiscard]]
        bool success() const;
        /**
         * @brief Returns the error code.
         * @return The stored `ErrorCode`.
         */
        [[nodiscard]]
        ErrorCode code() const;
        /**
         * @brief Returns the error message.
         * @return Reference to the stored message string.
         */
        [[nodiscard]]
        const std::string& message();
    private:
        ErrorCode m_code;
        std::string m_message;
    };

    [[nodiscard]]
    Status Success();
    [[nodiscard]]
    Status Empty(const std::string& message);
    [[nodiscard]]
    Status Null(const std::string& message);
    [[nodiscard]]
    Status OutOfMemory(const std::string& message);
    [[nodiscard]]
    Status InvalidArgument(const std::string& message);
    [[nodiscard]]
    Status FileNotFound(const std::string& message);
    [[nodiscard]]
    Status IO(const std::string& message);
} //namespace tlx

#endif //TLX_ERRORS_HPP