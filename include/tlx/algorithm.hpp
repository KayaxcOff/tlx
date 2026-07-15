//
// Created by muham on 15.07.2026.
//

#ifndef TLX_ALGORITHM_HPP
#define TLX_ALGORITHM_HPP

#include <tlx/concepts.hpp>

namespace tlx {
    /**
     * @brief Returns the greater of two values.
     *
     * @tparam T Type of the values (must support `operator>`).
     * @param x1 First value.
     * @param x2 Second value.
     * @return The larger of the two values.
     */
    template<typename T>
    [[nodiscard]]
    constexpr const T& max(const T& x1, const T& x2) noexcept {
        return (x1 > x2) ? x1 : x2;
    }
    /**
     * @brief Returns the smaller of two values.
     *
     * @tparam T Type of the values (must support `operator<`).
     * @param x1 First value.
     * @param x2 Second value.
     * @return The smaller of the two values.
     */
    template<typename T>
    [[nodiscard]]
    constexpr const T& min(const T& x1, const T& x2) noexcept {
        return (x1 < x2) ? x1 : x2;
    }
    /**
     * @brief Checks whether a value is within a closed range [low, high].
     *
     * @tparam T Type of the value.
     * @param value The value to check.
     * @param low   Lower bound (inclusive).
     * @param high  Upper bound (inclusive).
     * @return `true` if `value` is in range, otherwise `false`.
     */
    template<typename T>
    [[nodiscard]]
    constexpr bool in_range(const T& value, const T& low, const T& high) noexcept {
        return !(value < low) && !(high < value);
    }
    /**
     * @brief Checks if an integral value is zero.
     *
     * @tparam T Integral type.
     * @param x The value to check.
     * @return `true` if `x` is zero, otherwise `false`.
     */
    template<integral T>
    [[nodiscard]]
    constexpr bool isZero(T x) noexcept {
        return x == T{};
    }
    /**
     * @brief Checks if an integral value is positive.
     *
     * @tparam T Integral type.
     * @param x The value to check.
     * @return `true` if `x > 0`, otherwise `false`.
     */
    template<integral T>
    [[nodiscard]]
    constexpr bool isPositive(T x) noexcept {
        return x > T{};
    }
    /**
     * @brief Checks if an integral value is negative.
     *
     * @tparam T Integral type.
     * @param x The value to check.
     * @return `true` if `x < 0`, otherwise `false`.
     */
    template<integral T>
    [[nodiscard]]
    constexpr bool isNegative(T x) {
        return x < T{};
    }
    /**
     * @brief Checks if a value is between `start` and `end` (inclusive).
     *
     * @tparam T Integral type.
     * @param value The value to check.
     * @param start Lower bound (inclusive).
     * @param end   Upper bound (inclusive).
     * @return `true` if `value` is in [start, end], otherwise `false`.
     */
    template<integral T>
    [[nodiscard]]
    constexpr bool between(T value, T start, T end) noexcept {
        return start <= value && value <= end;
    }
} //namespace tlx

#endif //TLX_ALGORITHM_HPP