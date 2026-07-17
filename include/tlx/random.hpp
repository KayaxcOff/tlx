//
// Created by muham on 17.07.2026.
//

#ifndef TLX_RANDOM_HPP
#define TLX_RANDOM_HPP

#include <tlx/macros.hpp>

namespace tlx {
    class Alea {
    public:
        TLX_HD TLX_INLINE explicit Alea(const std::uint32_t seed) {
            this->m_seed = (seed == 0) ? 123456789U : seed;
        };
        TLX_HD TLX_INLINE Alea(const Alea &) = delete;
        TLX_HD TLX_INLINE Alea(Alea &&) noexcept = delete;
        TLX_HD TLX_INLINE ~Alea() = default;

        [[nodiscard]]
        TLX_HD TLX_INLINE std::uint32_t seed() const {
            return this->m_seed;
        }
        template<typename T>
        [[nodiscard]]
        T next() {
            std::uint32_t x = this->m_seed;
            x ^= x << 13;
            x ^= x >> 17;
            x ^= x << 5;
            this->m_seed = x;

            if constexpr (std::is_convertible_v<float, T> && !std::is_integral_v<T>) {
                float rand_f = static_cast<float>(x) / 4294967296.0f;
                return static_cast<T>(rand_f);
            } else {
                return static_cast<T>(x);
            }
        }
        template<typename T>
        [[nodiscard]]
        TLX_HD TLX_INLINE T uniform(const T min, const T max) {
            if constexpr (std::is_convertible_v<float, T> && !std::is_integral_v<T>) {
                return min + static_cast<T>(next<float>() * static_cast<float>(max - min));
            } else {
                const double r = static_cast<double>(next<std::uint32_t>()) / 4294967296.0;
                return min + static_cast<T>(r * (static_cast<double>(max) - static_cast<double>(min) + 1.0));
            }
        }

    private:
        std::uint32_t m_seed;
    };
} //namespace tlx

#endif //TLX_RANDOM_HPP