//
// Created by muham on 15.07.2026.
//

#ifndef TLX_CONCEPTS_HPP
#define TLX_CONCEPTS_HPP

#include <concepts>
#include <type_traits>

namespace tlx {
    /**
     * @brief Concept that checks if a type is nothrow constructible from itself.
     *
     * Requires that the type `Ty` can be copy or move constructed without throwing exceptions.
     *
     * @tparam Ty The type to check.
     */
    template<class Ty>
    concept no_throw_constructible = std::is_nothrow_constructible_v<Ty>;

    /**
     * @brief Concept that checks if a type is nothrow assignable.
     *
     * Requires that a lvalue of type `Ty` can be assigned from an rvalue of the same type
     * without throwing exceptions.
     *
     * @tparam Ty The type to check.
     */
    template<class Ty>
    concept no_throw_assignable = std::is_nothrow_assignable_v<Ty&, Ty&&>;

    /**
     * @brief Concept that checks if a type publicly inherits from a base class.
     *
     * This is a convenience concept wrapping `std::derived_from`.
     *
     * @tparam T    The derived type.
     * @tparam Base The base class to check inheritance from.
     */
    template<typename T, typename Base>
    concept extend = std::derived_from<T, Base>;

    /**
     * @brief Concept that checks if a type is an arithmetic type.
     *
     * Arithmetic types include integral and floating-point types.
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    /**
     * @brief Concept that checks if a type is an enumeration.
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept enum_type = std::is_enum_v<T>;

    /**
     * @brief Concept that checks if a type is trivially copyable.
     *
     * A trivially copyable type can be safely copied using `std::memcpy`.
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept trivially_copyable = std::is_trivially_copyable_v<T>;

    /**
     * @brief Concept that checks if a type is an integral type.
     *
     * @tparam Ty The type to check.
     */
    template <class Ty>
    concept integral = std::is_integral_v<Ty>;

    /**
     * @brief Concept that checks if a type is nothrow destructible and constructible from given arguments.
     *
     * Combines nothrow destructibility with constructibility from the provided argument types.
     *
     * @tparam Ty   The type to be constructed.
     * @tparam Args The argument types used for construction.
     */
    template<class Ty, class... Args>
    concept constructible_from = std::is_nothrow_destructible_v<Ty> && std::is_constructible_v<Ty, Args...>;
} //namespace tlx

#endif //TLX_CONCEPTS_HPP