//
// Created by muham on 15.07.2026.
//

#ifndef TLX_UTILITY_HPP
#define TLX_UTILITY_HPP

#include <tlx/concepts.hpp>
#include <tlx/xr.hpp>
#include <type_traits>

namespace tlx {
    /**
     * @brief Returns the actual address of the object or function referenced by `t`.
     *
     * @tparam Ty The type of the object.
     * @param t  Reference to the object.
     * @return Pointer to the object.
     */
    template <class Ty>
    [[nodiscard]]
    constexpr Ty* addressOf(Ty& t) noexcept {
        return __builtin_addressof(t);
    }

    /**
     * @brief Checks if `Base` is a base class of `Ty`.
     *
     * @tparam Base The potential base class.
     * @tparam Ty   The derived type to check.
     * @return `true` if `Base` is a base of `Ty`, otherwise `false`.
     */
    template<class Base, class Ty>
    [[nodiscard]]
    constexpr bool isBaseOf() {
        return __is_base_of(Base, Ty);
    }

    /**
     * @brief Converts an lvalue or rvalue to an rvalue reference.
     *
     * Enables move semantics by casting the argument to an rvalue reference.
     *
     * @tparam Ty  The type of the object.
     * @param Arg  The object to be moved.
     * @return An rvalue reference to the object.
     */
    template <class Ty>
    constexpr remove_reference_t<Ty>&& move(Ty&& Arg) {
        return static_cast<remove_reference_t<Ty>&&>(Arg);
    }

    /**
     * @brief Swaps the values of two objects.
     *
     * @tparam Ty Type of the objects being swapped. Must be nothrow constructible and assignable.
     * @param a First object to swap.
     * @param b Second object to swap.
     */
    template<class Ty>
    requires (tlx::no_throw_constructible<Ty> && tlx::no_throw_assignable<Ty>)
    constexpr void swap(Ty& a, Ty& b) noexcept(tlx::no_throw_constructible<Ty> && tlx::no_throw_assignable<Ty>) {
        Ty output = ::tlx::move(a);
        a = ::tlx::move(b);
        b = ::tlx::move(output);
    }

    /**
     * @brief Converts an enumeration value to its underlying integral type.
     *
     * @tparam Enum The enumeration type.
     * @param value The enumeration value to convert.
     * @return The underlying integral value.
     */
    template<class Enum>
    [[nodiscard]]
    constexpr underlying_type_t<Enum> toUnderlying(Enum value) noexcept {
        return static_cast<underlying_type_t<Enum>>(value);
    }

    /**
     * @brief Forwards an lvalue as either an lvalue or rvalue.
     *
     * Perfect forwarding utility.
     *
     * @tparam Ty  The type to forward.
     * @param Arg  The argument to forward.
     * @return Forwarded argument as lvalue or rvalue reference.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr Ty&& forward(remove_reference_t<Ty>& Arg) noexcept {
        return static_cast<Ty&&>(Arg);
    }

    /**
     * @brief Forwards a rvalue as a rvalue.
     *
     * Prevents forwarding of rvalues as lvalues.
     *
     * @tparam Ty  The type to forward.
     * @param Arg  The rvalue to forward.
     * @return Forwarded rvalue.
     */
    template <class Ty>
    [[nodiscard]]
    constexpr Ty&& forward(remove_reference_t<Ty>&& Arg) noexcept {
        static_assert(!std::is_lvalue_reference_v<Ty>, "bad forward call");
        return static_cast<Ty&&>(Arg);
    }

    /**
     * @brief Replaces the value of `obj` with `newValue` and returns the old value.
     *
     * @tparam Ty Type of the object.
     * @tparam U  Type of the new value (usually same as Ty).
     * @param obj      Object whose value will be replaced.
     * @param newValue New value to assign.
     * @return The old value of `obj` before replacement.
     */
    template<class Ty, class U = Ty>
    constexpr Ty exchange(Ty& obj, U&& newValue) {
        Ty output = ::tlx::move(obj);
        obj = ::tlx::forward<U>(newValue);
        return output;
    }

    /**
     * @brief A function object that returns its argument unchanged.
     *
     * This is a utility struct that implements the identity function.
     * It is useful in algorithms and functional programming where a no-op transformation is needed.
     *
     * Example usage:
     * @code
     * std::vector<int> v = {1, 2, 3};
     * auto it = std::find_if(v.begin(), v.end(), tlx::identity{});
     * @endcode
     */
    struct identity {
        template <class Ty>
        constexpr Ty&& operator()(Ty&& value) const noexcept {
            return ::tlx::forward<Ty>(value);
        }
    };
} // namespace tlx

#endif //TLX_UTILITY_HPP