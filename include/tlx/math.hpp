//
// Created by muham on 15.07.2026.
//

#ifndef TLX_MATH_HPP
#define TLX_MATH_HPP

#include <tlx/algorithm.hpp>

namespace tlx {
    /**
     * @brief Returns the sign of a value.
     *
     * @tparam T Arithmetic type.
     * @param value The value to check.
     * @return  1 if value is positive,
     *         -1 if value is negative,
     *          0 if value is zero.
     */
    template<typename  T>
    [[nodiscard]]
    constexpr int sign(T value) noexcept {
        return (T{} < value) - (value < T{});
    }
    /**
     * @brief Returns the absolute value of the given number.
     *
     * @tparam T Arithmetic type.
     * @param value The value to process.
     * @return The absolute value of `value`.
     */
    template<typename T>
    [[nodiscard]]
    constexpr T abs(T value) noexcept {
        return (value < 0) ? -value : value;
    }
    /**
     * @brief Performs linear interpolation between two values.
     *
     * Computes `a + t * (b - a)` with special handling to avoid overflow
     * when `a` and `b` have different signs, and clamping behavior when `t` is outside [0, 1].
     *
     * @tparam T Type of the values `a` and `b`.
     * @tparam U Type of the interpolation parameter `t` (usually floating point).
     * @param a Starting value.
     * @param b Ending value.
     * @param t Interpolation factor (typically in range [0, 1]).
     * @return The interpolated value between `a` and `b`.
     */
    template<typename T, typename U>
    [[nodiscard]]
    constexpr auto lerp(T a, T b, U t) noexcept {
        if ((a <= 0 && b >= 0) || (a >= 0 && b <= 0)) {
            return t * b + (1 - t) * a;
        }
        if (t == 1) {
            return b;
        }
        const auto x = a + t * (b - a);
        return (t > 1) == (b > a) ? ::tlx::max(b, x) : ::tlx::min(b, x);
    }
    /**
     * @brief Clamps a value between a lower and upper bound.
     *
     * @tparam T Type of the value.
     * @param value The value to clamp.
     * @param low   Lower bound.
     * @param high  Upper bound.
     * @return `value` if it is within [low, high], otherwise the nearest bound.
     */
    template<typename T>
    [[nodiscard]]
    constexpr const T& clamp(const T& value, const T& low, const T& high) noexcept {
        return (value < low) ? low : (high < value) ? high : value;
    }
} //namespace tlx

#endif //TLX_MATH_HPP