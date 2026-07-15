//
// Created by muham on 15.07.2026.
//

#ifndef TLX_XR_HPP
#define TLX_XR_HPP

namespace tlx {
    /**
     * @brief Removes reference qualifiers from a type.
     *
     * This is a type trait that provides the member typedef `type` which is the same as `Ty`
     * but with any reference qualifiers (`&` or `&&`) removed.
     *
     * @tparam Ty The type to remove reference from.
     */
    template<class Ty>
    struct RemoveReference {
        using type = Ty;
    };
    /**
     * @brief Removes lvalue reference qualifier.
     * @tparam Ty The type to remove reference from.
     */
    template<class Ty>
    struct RemoveReference<Ty&> {
        using type = Ty;
    };
    /**
     * @brief Removes rvalue reference qualifier.
     * @tparam Ty The type to remove reference from.
     */
    template<class Ty>
    struct RemoveReference<Ty&&> {
        using type = Ty;
    };

    /**
     * @brief Helper alias template for RemoveReference.
     *
     * @tparam Ty The type to remove reference from.
     */
    template<class Ty>
    using remove_reference_t = RemoveReference<Ty>::type;

    /**
     * @brief Provides the underlying integral type of enumeration.
     *
     * This is a type trait that provides the member typedef `type` which names the
     * underlying integral type of the enumeration `Ty`.
     *
     * @tparam Ty The enumeration type.
     * @tparam IsEnum Internal helper to enable this specialization only for enums.
     */
    template<class Ty, bool = __is_enum(Ty)>
    struct UnderlyingType {
        using type = __underlying_type(Ty);
    };
    /**
     * @brief Fallback specialization for non-enum types.
     *
     * This specialization exists to make the trait SFINAE-friendly and to avoid
     * compilation errors when used with non-enumeration types.
     *
     * @tparam Ty The type (non-enum).
     */
    template<class Ty>
    struct UnderlyingType<Ty, false> {};

    /**
     * @brief Helper alias template for UnderlyingType.
     *
     * @tparam Ty The enumeration type to get the underlying type of.
     */
    template<class Ty>
    using underlying_type_t = UnderlyingType<Ty>::type;
} //namespace tlx

#endif //TLX_XR_HPP