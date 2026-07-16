//
// Created by muham on 15.07.2026.
//

#ifndef TLX_TYPES_HPP
#define TLX_TYPES_HPP

#include <tlx/macros.hpp>
#include <bit>
#include <cstdint>
#if TLX_HAS_CUDA
#include <cuda_bf16.h>
#endif //#include <cuda_bf16.h>
#include <iosfwd>
#include <limits>

namespace tlx {
    using int8      = signed char;
    using int16     = short;
    using int32     = int;
    using int64     = long long;
    using uint8     = unsigned char;
    using uint16    = unsigned short;
    using uint32    = unsigned int;
    using uint64    = unsigned long long;
    using float32   = float;
    using float64   = double;
    using boolean   = bool;
    using size_t    = std::size_t;

    /**
     * @brief Brain Floating Point (bfloat16) 16-bit floating point type.
     *
     * This is a 16-bit floating point format with 8 bits for the exponent and 7 bits for the mantissa.
     * It is commonly used in machine learning due to its compatibility with 32-bit float in terms of dynamic range.
     */
    struct alignas(2) bfloat16 {
        TLX_HD bfloat16() = default;
        TLX_HD bfloat16(const float value) {
            auto f_bits = std::bit_cast<std::uint32_t>(value);
            f_bits += 0x7FFF + ((f_bits >> 16) & 1);
            this->value = static_cast<std::uint16_t>(f_bits >> 16);
        }
        #if TLX_HAS_CUDA
            TLX_HD bfloat16(const __nv_bfloat16 x) {
                this->value = __bfloat16_as_ushort(x);
            }
        #endif //#if TLX_HAS_CUDA
        TLX_HD bfloat16(const bfloat16& other) = default;
        TLX_HD bfloat16(bfloat16&& other) noexcept = default;
        TLX_HD ~bfloat16() = default;

        TLX_HD TLX_INLINE operator float() const {
            const std::uint32_t f_bits = static_cast<std::uint32_t>(this->value) << 16;
            return std::bit_cast<float>(f_bits);
        }
        #if TLX_HAS_CUDA
            TLX_HD TLX_INLINE operator __nv_bfloat16() const {
                return __ushort_as_bfloat16(this->value);
            }
        #endif //#if TLX_HAS_CUDA

        TLX_HD TLX_INLINE bfloat16 operator+(const bfloat16& other) const {
            #if defined(__CUDA_ARCH__)
                __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                return bfloat16(__hadd(a, b));
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) + static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD TLX_INLINE bfloat16 operator-(const bfloat16& other) const {
                #if defined(__CUDA_ARCH__)
                    __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                    __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                    return bfloat16(__hsub(a, b));
                #else //#if defined(__CUDA_ARCH__)
                    return static_cast<float>(*this) - static_cast<float>(other);
                #endif //#if defined(__CUDA_ARCH__) #else
                }
        TLX_HD TLX_INLINE bfloat16 operator*(const bfloat16& other) const {
                #if defined(__CUDA_ARCH__)
                    __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                    __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                    return bfloat16(__hmul(a, b));
                #else //#if defined(__CUDA_ARCH__)
                    return static_cast<float>(*this) * static_cast<float>(other);
                #endif //#if defined(__CUDA_ARCH__) #else
            }
        TLX_HD TLX_INLINE bfloat16 operator/(const bfloat16& other) const {
                #if defined(__CUDA_ARCH__)
                    __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                    __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                return bfloat16(__hdiv(a, b));
                #else //#if defined(__CUDA_ARCH__)
                    return static_cast<float>(*this) / static_cast<float>(other);
                #endif //#if defined(__CUDA_ARCH__) #else
            }

        TLX_HD TLX_INLINE bfloat16& operator+=(const bfloat16& other) {
                *this = *this + other;
                return *this;
            }
        TLX_HD TLX_INLINE bfloat16& operator-=(const bfloat16& other) {
                *this = *this - other;
                return *this;
            }
        TLX_HD TLX_INLINE bfloat16& operator*=(const bfloat16& other) {
                *this = *this * other;
                return *this;
            }
        TLX_HD TLX_INLINE bfloat16& operator/=(const bfloat16& other) {
                *this = *this / other;
                return *this;
            }

        TLX_HD TLX_INLINE bool operator==(const bfloat16& other) const {
                return static_cast<float>(*this) == static_cast<float>(other);
            }
        TLX_HD TLX_INLINE bool operator!=(const bfloat16& other) const {
                return !(*this == other);
            }
        TLX_HD TLX_INLINE bool operator<(const bfloat16& other) const {
                return static_cast<float>(*this) < static_cast<float>(other);
            }
        TLX_HD TLX_INLINE bool operator<=(const bfloat16& other) const {
                return static_cast<float>(*this) <= static_cast<float>(other);
            }
        TLX_HD TLX_INLINE bool operator>(const bfloat16& other) const {
                return static_cast<float>(*this) > static_cast<float>(other);
            }
        TLX_HD TLX_INLINE bool operator>=(const bfloat16& other) const {
                return static_cast<float>(*this) >= static_cast<float>(other);
            }

        TLX_HD TLX_INLINE bfloat16& operator=(const bfloat16& other) = default;
        TLX_HD TLX_INLINE bfloat16& operator=(bfloat16&& other) noexcept = default;
    private:
        std::uint16_t value;
    };

    std::ostream& operator<<(std::ostream& os, const bfloat16& rhs);

    /**
     * @brief IEEE 754 half-precision floating-point type (fp16).
     *
     * 16-bit floating point format with 5 bits exponent and 10 bits mantissa.
     * Provides CUDA interoperability when available.
     */
    struct alignas(2) half {
        TLX_HD half() : value(0) {}

        TLX_HD TLX_INLINE half(const float f) {
            #if defined(__CUDA_ARCH__)
                this->value = __half_as_ushort(__float2half_rn(f));
            #else //#if defined(__CUDA_ARCH__)
                this->value = float_to_half_bits(f);
            #endif //#if defined(__CUDA_ARCH__) #else
        }

        #if TLX_HAS_CUDA
            TLX_HD TLX_INLINE half(const __half x) {
                this->value = __half_as_ushort(x);
            }
            TLX_HD TLX_INLINE operator __half() const {
                return __ushort_as_half(this->value);
            }
        #endif //#if TLX_HAS_CUDA

        TLX_HD TLX_INLINE operator float() const {
            #if defined(__CUDA_ARCH__)
                return __half2float(__ushort_as_half(this->value));
            #else //#if defined(__CUDA_ARCH__)
                return half_bits_to_float(this->value);
            #endif //#if defined(__CUDA_ARCH__) #else
        }

        [[nodiscard]]
        TLX_HD TLX_INLINE uint16_t raw() const { return this->value; }

        TLX_HD TLX_INLINE half operator+(const half& other) const {
            #if defined(__CUDA_ARCH__)
                return __hadd(static_cast<__half>(*this), static_cast<__half>(other));
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) + static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__)
        }
        TLX_HD TLX_INLINE half operator-(const half& other) const {
            #if defined(__CUDA_ARCH__)
                return __hsub(static_cast<__half>(*this), static_cast<__half>(other));
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) - static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD TLX_INLINE half operator*(const half& other) const {
            #if defined(__CUDA_ARCH__)
                return __hmul(static_cast<__half>(*this), static_cast<__half>(other));
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) * static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD TLX_INLINE half operator/(const half& other) const {
            #if defined(__CUDA_ARCH__)
                return __hdiv(static_cast<__half>(*this), static_cast<__half>(other));
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) / static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }

        TLX_HD TLX_INLINE half& operator+=(const half& other) { *this = *this + other; return *this; }
        TLX_HD TLX_INLINE half& operator-=(const half& other) { *this = *this - other; return *this; }
        TLX_HD TLX_INLINE half& operator*=(const half& other) { *this = *this * other; return *this; }
        TLX_HD TLX_INLINE half& operator/=(const half& other) { *this = *this / other; return *this; }

        TLX_HD TLX_INLINE bool operator==(const half& other) const { return static_cast<float>(*this) == static_cast<float>(other); }
        TLX_HD TLX_INLINE bool operator!=(const half& other) const { return !(*this == other); }
        TLX_HD TLX_INLINE bool operator<(const half& other) const { return static_cast<float>(*this) < static_cast<float>(other); }
        TLX_HD TLX_INLINE bool operator<=(const half& other) const { return static_cast<float>(*this) <= static_cast<float>(other); }
        TLX_HD TLX_INLINE bool operator>(const half& other) const { return static_cast<float>(*this) > static_cast<float>(other); }
        TLX_HD TLX_INLINE bool operator>=(const half& other) const { return static_cast<float>(*this) >= static_cast<float>(other); }

    private:
        uint16_t value;

        TLX_HD TLX_INLINE static uint16_t float_to_half_bits(const float f) {
            const auto x = std::bit_cast<uint32_t>(f);
            const uint32_t sign = (x >> 16) & 0x8000u;
            int32_t exp = static_cast<int32_t>((x >> 23) & 0xFFu) - 127 + 15;
            uint32_t mantissa = x & 0x7FFFFFu;

            // Inf / NaN
            if (((x >> 23) & 0xFFu) == 0xFFu) {
                return static_cast<uint16_t>(sign | (mantissa != 0 ? 0x7E00u : 0x7C00u));
            }
            if (exp >= 0x1F) {
                return static_cast<uint16_t>(sign | 0x7C00u);
            }
            if (exp <= 0) {
                if (exp < -10) {
                    return static_cast<uint16_t>(sign);
                }
                mantissa |= 0x800000u;
                const int shift = 14 - exp;
                uint32_t half_mantissa = mantissa >> shift;
                const uint32_t remainder = mantissa & ((1u << shift) - 1u);
                const uint32_t halfway = 1u << (shift - 1);
                if (remainder > halfway || (remainder == halfway && (half_mantissa & 1u))) {
                    half_mantissa++;
                }
                return static_cast<uint16_t>(sign | half_mantissa);
            }
            uint32_t half_mantissa = mantissa >> 13;
            const uint32_t remainder = mantissa & 0x1FFFu;
            if (remainder > 0x1000u || (remainder == 0x1000u && (half_mantissa & 1u))) {
                half_mantissa++;
                if (half_mantissa == 0x400u) {
                    half_mantissa = 0;
                    exp++;
                    if (exp >= 0x1F) {
                        return static_cast<uint16_t>(sign | 0x7C00u);
                    }
                }
            }
            return static_cast<uint16_t>(sign | (static_cast<uint32_t>(exp) << 10) | half_mantissa);
        }

        TLX_HD TLX_INLINE static float half_bits_to_float(const uint16_t h) {
            const uint32_t sign = static_cast<uint32_t>(h & 0x8000u) << 16;
            uint32_t exp = (h >> 10) & 0x1Fu;
            uint32_t mantissa = h & 0x3FFu;

            if (exp == 0) {
                if (mantissa == 0) {
                    return std::bit_cast<float>(sign);
                }
                exp = 1;
                while ((mantissa & 0x400u) == 0) {
                    mantissa <<= 1;
                    exp--;
                }
                mantissa &= 0x3FFu;
                const uint32_t f_exp = (exp - 15u + 127u) << 23;
                const uint32_t f_mantissa = mantissa << 13;
                return std::bit_cast<float>(sign | f_exp | f_mantissa);
            }
            if (exp == 0x1Fu) {
                return std::bit_cast<float>(sign | 0x7F800000u | (mantissa << 13));
            }
            const uint32_t f_exp = (exp - 15u + 127u) << 23;
            const uint32_t f_mantissa = mantissa << 13;
            return std::bit_cast<float>(sign | f_exp | f_mantissa);
        }
    };

    std::ostream& operator<<(std::ostream& os, const half& rhs);

    /**
     * @brief Quantized 16-bit signed integer (qint16).
     *
     * This type is typically used in quantized neural network computations.
     * It provides saturation when constructed from larger integer types and
     * promotes arithmetic results to 32-bit integers to prevent overflow.
     */
    struct alignas(2) qint16 {
        TLX_HD qint16() : value(0) {}
        TLX_HD TLX_INLINE explicit qint16(const std::int16_t v) : value(v) {}
        TLX_HD TLX_INLINE explicit qint16(const std::int32_t v) {
            std::int32_t clamped = v;
            if (clamped < std::numeric_limits<std::int16_t>::min())
                clamped = std::numeric_limits<std::int16_t>::min();
            if (clamped > std::numeric_limits<std::int16_t>::max())
                clamped = std::numeric_limits<std::int16_t>::max();
            this->value = static_cast<std::int16_t>(clamped);
        }

        TLX_HD TLX_INLINE explicit operator std::int32_t() const {
            return this->value;
        }

        [[nodiscard]]
        TLX_HD TLX_INLINE std::int16_t raw() const {
            return this->value;
        }

        TLX_HD TLX_INLINE std::int32_t operator+(const qint16& other) const {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE std::int32_t operator-(const qint16& other) const {
            return static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE std::int32_t operator*(const qint16& other) const {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }

        TLX_HD TLX_INLINE bool operator==(const qint16& other) const {
            return this->value == other.value;
        }
        TLX_HD TLX_INLINE bool operator!=(const qint16& other) const {
            return !(*this == other);
        }
        TLX_HD TLX_INLINE bool operator<(const qint16& other) const {
            return this->value < other.value;
        }
        TLX_HD TLX_INLINE bool operator<=(const qint16& other) const {
            return this->value <= other.value;
        }
        TLX_HD TLX_INLINE bool operator>(const qint16& other) const {
            return this->value > other.value;
        }
        TLX_HD TLX_INLINE bool operator>=(const qint16& other) const {
            return this->value >= other.value;
        }

    private:
        std::int16_t value;
    };

    std::ostream& operator<<(std::ostream& os, const qint16& rhs);

    /**
     * @brief Quantized 8-bit signed integer (qint8).
     *
     * This type is typically used in quantized neural network models.
     * It provides saturation on construction from larger integer types and
     * promotes arithmetic operations to 32-bit integers to avoid overflow.
     */
    struct alignas(1) qint8 {
        TLX_HD qint8() : value(0) {}
        TLX_HD qint8(const std::int8_t v) : value(v) {}
        TLX_HD qint8(const std::int32_t v) {
            std::int32_t clamped = v;
            if (clamped < std::numeric_limits<std::int8_t>::min())
                clamped = std::numeric_limits<std::int8_t>::min();
            if (clamped > std::numeric_limits<std::int8_t>::max())
                clamped = std::numeric_limits<std::int8_t>::max();
            this->value = static_cast<std::int8_t>(clamped);
        }

        [[nodiscard]]
        TLX_HD TLX_INLINE std::int8_t raw() const {
            return this->value;
        }

        TLX_HD TLX_INLINE explicit operator std::int32_t() const {
            return this->value;
        }
        TLX_HD TLX_INLINE std::int32_t operator+(const qint8& other) const {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE int32_t operator-(const qint8& other) const {
            return static_cast<int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE std::int32_t operator*(const qint8& other) const {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }

        TLX_HD TLX_INLINE bool operator==(const qint8& other) const {
            return this->value == other.value;
        }
        TLX_HD TLX_INLINE bool operator!=(const qint8& other) const {
            return !(*this == other);
        }
        TLX_HD TLX_INLINE bool operator<(const qint8& other) const {
            return this->value < other.value;
        }
        TLX_HD TLX_INLINE bool operator<=(const qint8& other) const {
            return this->value <= other.value;
        }
        TLX_HD TLX_INLINE bool operator>(const qint8& other) const {
            return this->value > other.value;
        }
        TLX_HD TLX_INLINE bool operator>=(const qint8& other) const {
            return this->value >= other.value;
        }
    private:
        std::int8_t value;
    };

    std::ostream& operator<<(std::ostream& os, const qint8& rhs);

    /**
     * @brief Quantized 16-bit unsigned integer (quint16).
     *
     * This type is typically used in quantized neural network computations.
     * It provides saturation (clamping) when constructed from larger integer types.
     */
    struct alignas(2) quint16 {
        TLX_HD quint16() : value(0) {}
        TLX_HD TLX_INLINE explicit quint16(const std::uint16_t v) : value(v) {}
        TLX_HD TLX_INLINE explicit quint16(const std::int32_t v) {
            std::int32_t clamped = v;
            if (clamped < 0)
                clamped = 0;
            if (clamped > std::numeric_limits<std::uint16_t>::max())
                clamped = std::numeric_limits<std::uint16_t>::max();
            this->value = static_cast<std::uint16_t>(clamped);
        }

        TLX_HD TLX_INLINE explicit operator std::int32_t() const {
            return this->value;
        }

        [[nodiscard]]
        TLX_HD TLX_INLINE std::uint16_t raw() const {
            return this->value;
        }

        TLX_HD TLX_INLINE std::int32_t operator+(const quint16& other) const {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE std::int32_t operator-(const quint16& other) const {
            return static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE std::int32_t operator*(const quint16& other) const {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }

        TLX_HD TLX_INLINE bool operator==(const quint16& other) const {
            return this->value == other.value;
        }
        TLX_HD TLX_INLINE bool operator!=(const quint16& other) const {
            return !(*this == other);
        }
        TLX_HD TLX_INLINE bool operator<(const quint16& other) const {
            return this->value < other.value;
        }
        TLX_HD TLX_INLINE bool operator<=(const quint16& other) const {
            return this->value <= other.value;
        }
        TLX_HD TLX_INLINE bool operator>(const quint16& other) const {
            return this->value > other.value;
        }
        TLX_HD TLX_INLINE bool operator>=(const quint16& other) const {
            return this->value >= other.value;
        }

    private:
        std::uint16_t value;
    };

    std::ostream& operator<<(std::ostream& os, const quint16& rhs);

    /**
     * @brief Quantized 8-bit unsigned integer (quint8).
     *
     * This type is typically used in quantized neural network models.
     * It provides saturation (clamping) when constructed from larger integer types.
     */
    struct alignas(1) quint8 {
        TLX_HD quint8() : value(0) {}
        TLX_HD TLX_INLINE explicit quint8(const std::uint8_t v) : value(v) {}
        TLX_HD TLX_INLINE explicit quint8(const std::int32_t v) {
            std::int32_t clamped = v;
            if (clamped < 0)
                clamped = 0;
            if (clamped > std::numeric_limits<std::uint8_t>::max())
                clamped = std::numeric_limits<std::uint8_t>::max();
            this->value = static_cast<uint8_t>(clamped);
        }

        [[nodiscard]]
        TLX_HD TLX_INLINE std::uint8_t raw() const {
            return this->value;
        }

        TLX_HD TLX_INLINE explicit operator std::int32_t() const {
            return this->value;
        }

        TLX_HD TLX_INLINE std::int32_t operator+(const quint8& other) const {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE std::int32_t operator-(const quint8& other) const {
            return static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD TLX_INLINE std::int32_t operator*(const quint8& other) const {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }

        TLX_HD TLX_INLINE bool operator==(const quint8& other) const {
            return this->value == other.value;
        }
        TLX_HD TLX_INLINE bool operator!=(const quint8& other) const {
            return !(*this == other);
        }
        TLX_HD TLX_INLINE bool operator<(const quint8& other) const {
            return this->value < other.value;
        }
        TLX_HD TLX_INLINE bool operator<=(const quint8& other) const {
            return this->value <= other.value;
        }
        TLX_HD TLX_INLINE bool operator>(const quint8& other) const {
            return this->value > other.value;
        }
        TLX_HD TLX_INLINE bool operator>=(const quint8& other) const {
            return this->value >= other.value;
        }

    private:
        std::uint8_t value;
    };

    std::ostream& operator<<(std::ostream& os, const quint8& rhs);
} //namespace tlx

#include <format>

namespace std {
    template<>
    struct formatter<tlx::bfloat16> : formatter<float> {
        auto format(const tlx::bfloat16& value, format_context& ctx) const {
            return formatter<float>::format(value, ctx);
        }
    };

    template<>
    struct formatter<tlx::half> : formatter<float> {
        auto format(const tlx::half& value, format_context& ctx) const {
            return formatter<float>::format(value, ctx);
        }
    };

    template<>
    struct formatter<tlx::qint8> : formatter<std::int32_t> {
        auto format(const tlx::qint8& value, format_context& ctx) const {
            return formatter<std::int32_t>::format(static_cast<std::int32_t>(value), ctx);
        }
    };

    template<>
    struct formatter<tlx::quint8> : formatter<std::int32_t> {
        auto format(const tlx::quint8& value, format_context& ctx) const {
            return formatter<std::int32_t>::format(static_cast<std::int32_t>(value), ctx);
        }
    };

    template<>
    struct formatter<tlx::qint16> : formatter<std::int32_t> {
        auto format(const tlx::qint16& value, format_context& ctx) const {
            return formatter<std::int32_t>::format(static_cast<std::int32_t>(value), ctx);
        }
    };

    template<>
    struct formatter<tlx::quint16> : formatter<std::int32_t> {
        auto format(const tlx::quint16& value, format_context& ctx) const {
            return formatter<std::int32_t>::format(static_cast<std::int32_t>(value), ctx);
        }
    };
} //namespace std

#endif //TLX_TYPES_HPP