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
#include <immintrin.h>

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
        std::uint16_t value;

        TLX_HD TLX_INLINE static std::uint16_t float_to_half_bits(const float f) {
            const __m128 f_vec = _mm_set_ss(f);
            const __m128i h_vec = _mm_cvtps_ph(f_vec, _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC);
            return static_cast<std::uint16_t>(_mm_cvtsi128_si32(h_vec));
        }

        TLX_HD TLX_INLINE static float half_bits_to_float(const std::uint16_t h) {
            const std::uint32_t sign = (h & 0x8000u) << 16;
            std::uint32_t exp = (h >> 10) & 0x1Fu;
            std::uint32_t mantissa = h & 0x3FFu;

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
                const std::uint32_t f_exp = (exp - 15u + 127u) << 23;
                const std::uint32_t f_mantissa = mantissa << 13;
                return std::bit_cast<float>(sign | f_exp | f_mantissa);
            }
            if (exp == 0x1Fu) {
                return std::bit_cast<float>(sign | 0x7F800000u | (mantissa << 13));
            }
            const std::uint32_t f_exp = (exp - 15u + 127u) << 23;
            const std::uint32_t f_mantissa = mantissa << 13;
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

    struct vec8f {
        TLX_INLINE vec8f() : reg(_mm256_setzero_ps()) {}
        TLX_INLINE vec8f(const float* x) : reg(_mm256_loadu_ps(x)) {}
        TLX_INLINE vec8f(const __m256 x) : reg(x) {}
        TLX_INLINE vec8f(const vec8f&) = default;
        TLX_INLINE vec8f(const vec8f&&) noexcept = delete;

        TLX_INLINE void store(float* x) const {
            _mm256_storeu_ps(x, reg);
        }

        TLX_INLINE operator __m256() const {
            return this->reg;
        }

        TLX_INLINE vec8f operator+(const vec8f& other) const {
            return _mm256_add_ps(this->reg, other.reg);
        }
        TLX_INLINE vec8f operator-(const vec8f& other) const {
            return _mm256_sub_ps(this->reg, other.reg);
        }
        TLX_INLINE vec8f operator*(const vec8f& other) const {
            return _mm256_mul_ps(this->reg, other.reg);
        }
        TLX_INLINE vec8f operator/(const vec8f& other) const {
            return _mm256_div_ps(this->reg, other.reg);
        }

        TLX_INLINE vec8f& operator+=(const vec8f& other) {
            this->reg = _mm256_add_ps(this->reg, other.reg);
            return *this;
        }
        vec8f& operator-=(const vec8f& other) {
            this->reg = _mm256_sub_ps(this->reg, other.reg);
            return *this;
        }
        vec8f& operator*=(const vec8f& other) {
            this->reg = _mm256_mul_ps(this->reg, other.reg);
            return *this;
        }
        vec8f& operator/=(const vec8f& other) {
            this->reg = _mm256_div_ps(this->reg, other.reg);
            return *this;
        }

        vec8f operator==(const vec8f& other) const {
            return _mm256_cmp_ps(this->reg, other.reg, _CMP_EQ_OQ);
        }
        vec8f operator!=(const vec8f& other) const {
            return _mm256_cmp_ps(this->reg, other.reg, _CMP_NEQ_OQ);
        }
        vec8f operator<(const vec8f& other) const {
            return _mm256_cmp_ps(this->reg, other.reg, _CMP_LT_OQ);
        }
        vec8f operator<=(const vec8f& other) const {
            return _mm256_cmp_ps(this->reg, other.reg, _CMP_LE_OQ);
        }
        vec8f operator>(const vec8f& other) const {
            return _mm256_cmp_ps(this->reg, other.reg, _CMP_GT_OQ);
        }
        vec8f operator>=(const vec8f& other) const {
            return _mm256_cmp_ps(this->reg, other.reg, _CMP_GE_OQ);
        }

        vec8f& operator=(const vec8f&) = default;
        vec8f& operator=(const vec8f&&) noexcept = delete;
    private:
        __m256 reg;
    };

    struct vec8i {
        TLX_INLINE vec8i() : reg(_mm256_setzero_si256()) {}
        TLX_INLINE vec8i(const std::int32_t* x) : reg(_mm256_loadu_si256(reinterpret_cast<const __m256i *>(x))) {}
        TLX_INLINE vec8i(const __m256i x) : reg(x) {}

        TLX_INLINE void store(std::int32_t* x) const {
            _mm256_storeu_si256(reinterpret_cast<__m256i *>(x), this->reg);
        }

        TLX_INLINE operator __m256i() const {
            return this->reg;
        }

        TLX_INLINE vec8i operator+(const vec8i& other) const {
            return _mm256_add_epi32(this->reg, other.reg);
        }
        TLX_INLINE vec8i operator-(const vec8i& other) const {
            return _mm256_sub_epi32(this->reg, other.reg);
        }
        TLX_INLINE vec8i operator*(const vec8i& other) const {
            return _mm256_mullo_epi32(this->reg, other.reg);
        }
        TLX_INLINE vec8i operator/(const vec8i& other) const {
            return _mm256_div_epi32(this->reg, other.reg);
        }

        TLX_INLINE vec8i& operator+=(const vec8i& other) {
            this->reg = _mm256_add_epi32(this->reg, other.reg);
            return *this;
        }
        TLX_INLINE vec8i& operator-=(const vec8i& other) {
            this->reg = _mm256_sub_epi32(this->reg, other.reg);
            return *this;
        }
        TLX_INLINE vec8i& operator*=(const vec8i& other) {
            this->reg = _mm256_mullo_epi32(this->reg, other.reg);
            return *this;
        }
        TLX_INLINE vec8i& operator/=(const vec8i& other) {
            this->reg = _mm256_div_epi32(this->reg, other.reg);
            return *this;
        }

        TLX_INLINE vec8i operator==(const vec8i& other) const {
            return _mm256_cmpeq_epi32(this->reg, other.reg);
        }
        TLX_INLINE vec8i operator!=(const vec8i& other) const {
            const __m256i eq = _mm256_cmpeq_epi32(this->reg, other.reg);
            return _mm256_xor_si256(eq, _mm256_set1_epi32(-1));
        }
        TLX_INLINE vec8i operator<(const vec8i& other) const {
            return _mm256_cmpgt_epi32(this->reg, other.reg);
        }
        TLX_INLINE vec8i operator<=(const vec8i& other) const {
            const __m256i gt = _mm256_cmpgt_epi32(this->reg, other.reg);
            return _mm256_xor_si256(gt, _mm256_set1_epi32(-1));
        }
        TLX_INLINE vec8i operator>(const vec8i& other) const {
            return _mm256_cmpgt_epi32(this->reg, other.reg);
        }
        TLX_INLINE vec8i operator>=(const vec8i& other) const {
            const __m256i lt = _mm256_cmpgt_epi32(other.reg, this->reg);
            return _mm256_xor_si256(lt, _mm256_set1_epi32(-1));
        }

        vec8i& operator=(const vec8i&) = default;
        vec8i& operator=(const vec8i&&) noexcept = delete;
    private:
        __m256i reg;
    };

    struct vec4d {
        TLX_INLINE vec4d() : reg(_mm256_setzero_pd()) {}
        TLX_INLINE vec4d(const double* x) : reg(_mm256_loadu_pd(x)) {}
        TLX_INLINE vec4d(const __m256d x) : reg(x) {}
        TLX_INLINE vec4d(const vec4d&) = default;
        TLX_INLINE vec4d(const vec4d&&) noexcept = delete;

        TLX_INLINE void store(double* x) const {
            _mm256_store_pd(x, this->reg);
        }

        TLX_INLINE operator __m256d() const {
            return this->reg;
        }

        TLX_INLINE vec4d operator+(const vec4d& other) const {
            return _mm256_add_pd(this->reg, other.reg);
        }
        TLX_INLINE vec4d operator-(const vec4d& other) const {
            return _mm256_sub_pd(this->reg, other.reg);
        }
        vec4d operator*(const vec4d& other) const {
            return _mm256_mul_pd(this->reg, other.reg);
        }
        vec4d operator/(const vec4d& other) const {
            return _mm256_div_pd(this->reg, other.reg);
        }

        TLX_INLINE vec4d& operator+=(const vec4d& other) {
            this->reg = _mm256_add_pd(this->reg, other.reg);
            return *this;
        }
        TLX_INLINE vec4d& operator-=(const vec4d& other) {
            this->reg = _mm256_sub_pd(this->reg, other.reg);
            return *this;
        }
        TLX_INLINE vec4d& operator*=(const vec4d& other) {
            this->reg = _mm256_mul_pd(this->reg, other.reg);
            return *this;
        }
        TLX_INLINE vec4d& operator/=(const vec4d& other) {
            this->reg = _mm256_div_pd(this->reg, other.reg);
            return *this;
        }

        TLX_INLINE vec4d operator==(const vec4d& other) const {
            return _mm256_cmp_pd(this->reg, other.reg, _CMP_EQ_OQ);
        }
        TLX_INLINE vec4d operator!=(const vec4d& other) const {
            return _mm256_cmp_pd(this->reg, other.reg, _CMP_NEQ_OQ);
        }
        TLX_INLINE vec4d operator<(const vec4d& other) const {
            return _mm256_cmp_pd(this->reg, other.reg, _CMP_LT_OQ);
        }
        TLX_INLINE vec4d operator<=(const vec4d& other) const {
            return _mm256_cmp_pd(this->reg, other.reg, _CMP_LE_OQ);
        }
        vec4d operator>(const vec4d& other) const {
            return _mm256_cmp_pd(this->reg, other.reg, _CMP_GT_OQ);
        }
        vec4d operator>=(const vec4d& other) const {
            return _mm256_cmp_pd(this->reg, other.reg, _CMP_GE_OQ);
        }

        vec4d& operator=(const vec4d&) = default;
        vec4d& operator=(const vec4d&&) noexcept = delete;
    private:
        __m256d reg;
    };

    struct vec16bf {
        TLX_INLINE vec16bf() : reg(_mm256_setzero_si256()) {}
        TLX_INLINE vec16bf(const bfloat16* x) : reg(_mm256_setzero_si256()) {
            this->reg = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(x));
        }
        TLX_INLINE vec16bf(const __m256i x) : reg(_mm256_setzero_si256()) {
            this->reg = x;
        }

        TLX_INLINE void store(bfloat16* x) const {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(x), this->reg);
        }

        TLX_INLINE operator __m256i() const {
            return this->reg;
        }

        TLX_INLINE vec16bf operator+(const vec16bf& other) const {
            const vec8f r_lo = widen_low(this->reg) + widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) + widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }
        TLX_INLINE vec16bf operator-(const vec16bf& other) const {
            const vec8f r_lo = widen_low(this->reg) - widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) - widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }
        TLX_INLINE vec16bf operator*(const vec16bf& other) const {
            const vec8f r_lo = widen_low(this->reg) * widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) * widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }
        TLX_INLINE vec16bf operator/(const vec16bf& other) const {
            const vec8f r_lo = widen_low(this->reg) / widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) / widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }

        TLX_INLINE vec16bf& operator+=(const vec16bf& other) { *this = *this + other; return *this; }
        TLX_INLINE vec16bf& operator-=(const vec16bf& other) { *this = *this - other; return *this; }
        TLX_INLINE vec16bf& operator*=(const vec16bf& other) { *this = *this * other; return *this; }
        TLX_INLINE vec16bf& operator/=(const vec16bf& other) { *this = *this / other; return *this; }

    private:
        __m256i reg;

        TLX_INLINE static vec8f widen_low(const __m256i x) {
            const __m128i lo16 = _mm256_castsi256_si128(x);
            const __m256i lo32 = _mm256_cvtepu16_epi32(lo16);
            const __m256i shifted = _mm256_slli_epi32(lo32, 16);
            return _mm256_castsi256_ps(shifted);
        }
        TLX_INLINE static vec8f widen_high(const __m256i x) {
            const __m128i hi16 = _mm256_extracti128_si256(x, 1);
            const __m256i hi32 = _mm256_cvtepu16_epi32(hi16);
            const __m256i shifted = _mm256_slli_epi32(hi32, 16);
            return _mm256_castsi256_ps(shifted);
        }

        TLX_INLINE static __m256i narrow_and_combine(const vec8f &lo, const vec8f &hi) {
            const __m256i lo_bits = _mm256_castps_si256(lo);
            const __m256i hi_bits = _mm256_castps_si256(hi);

            const __m256i lo_round = _mm256_add_epi32(
                lo_bits,
                _mm256_add_epi32(_mm256_set1_epi32(0x7FFF),
                    _mm256_and_si256(_mm256_srli_epi32(lo_bits, 16), _mm256_set1_epi32(1))));
            const __m256i hi_round = _mm256_add_epi32(
                hi_bits,
                _mm256_add_epi32(_mm256_set1_epi32(0x7FFF),
                    _mm256_and_si256(_mm256_srli_epi32(hi_bits, 16), _mm256_set1_epi32(1))));

            const __m256i lo_shifted = _mm256_srli_epi32(lo_round, 16);
            const __m256i hi_shifted = _mm256_srli_epi32(hi_round, 16);

            const __m256i packed = _mm256_packus_epi32(lo_shifted, hi_shifted);
            return _mm256_permute4x64_epi64(packed, 0xD8);
        }
    };

    struct vec16h {
        TLX_INLINE vec16h() : reg(_mm256_setzero_si256()) {}

        TLX_INLINE explicit vec16h(const half* x) : reg(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(x))) {}

        TLX_INLINE vec16h(const __m256i x) : reg(x) {}

        TLX_INLINE void store(half* x) const {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(x), this->reg);
        }

        TLX_INLINE operator __m256i() const { return this->reg; }

        TLX_INLINE static vec8f widen_low(const __m256i x) {
            const __m128i lo = _mm256_castsi256_si128(x);
            return _mm256_cvtph_ps(lo);
        }

        TLX_INLINE static vec8f widen_high(const __m256i x) {
            const __m128i hi = _mm256_extracti128_si256(x, 1);
            return _mm256_cvtph_ps(hi);
        }

        TLX_INLINE static __m256i narrow_and_combine(const vec8f lo, const vec8f hi) {
            const __m128i lo_h = _mm256_cvtps_ph(lo, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
            const __m128i hi_h = _mm256_cvtps_ph(hi, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
            return _mm256_set_m128i(hi_h, lo_h);
        }

        TLX_INLINE vec16h operator+(const vec16h& other) const {
            const vec8f r_lo = widen_low(this->reg) + widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) + widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }
        TLX_INLINE vec16h operator-(const vec16h& other) const {
            const vec8f r_lo = widen_low(this->reg) - widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) - widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }
        TLX_INLINE vec16h operator*(const vec16h& other) const {
            const vec8f r_lo = widen_low(this->reg) * widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) * widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }
        TLX_INLINE vec16h operator/(const vec16h& other) const {
            const vec8f r_lo = widen_low(this->reg) / widen_low(other.reg);
            const vec8f r_hi = widen_high(this->reg) / widen_high(other.reg);
            return narrow_and_combine(r_lo, r_hi);
        }

        TLX_INLINE vec16h& operator+=(const vec16h& other) { *this = *this + other; return *this; }
        TLX_INLINE vec16h& operator-=(const vec16h& other) { *this = *this - other; return *this; }
        TLX_INLINE vec16h& operator*=(const vec16h& other) { *this = *this * other; return *this; }
        TLX_INLINE vec16h& operator/=(const vec16h& other) { *this = *this / other; return *this; }

    private:
        __m256i reg;
    };
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