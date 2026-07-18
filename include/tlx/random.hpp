//
// Created by muham on 17.07.2026.
//

#ifndef TLX_RANDOM_HPP
#define TLX_RANDOM_HPP

#include <tlx/concepts.hpp>
#include <tlx/macros.hpp>
#include <cmath>

namespace tlx {
    /**
     * @brief Philox-4x32-10 pseudo-random number generator.
     *
     * A high-quality, counter-based RNG suitable for parallel and GPU use.
     * It is fast, has good statistical properties, and is deterministic.
     */
    class philox4x32 {
    public:
        /**
         * @brief Constructs the generator with the given seed.
         *
         * @param seed 64-bit seed value.
         */
        TLX_HD explicit philox4x32(const std::uint64_t seed) noexcept : m_counter({0, 0, 0, 0}), m_key({static_cast<std::uint32_t>(seed),static_cast<std::uint32_t>(seed >> 32)}) {}

        /**
         * @brief Generates the next random integer.
         *
         * @tparam T Integral type (up to 64-bit).
         * @return Random integer value.
         */
        template<integral T>
        [[nodiscard]]
        TLX_HD T next() noexcept {
            constexpr std::size_t words_needed = (sizeof(T) + sizeof(std::uint32_t) - 1) / sizeof(std::uint32_t);
            static_assert(words_needed >= 1 && words_needed <= 2,
                "philox4x32::next only integer types up to 64 bits");

            if (this->m_index + words_needed > 4) {
                refill();
            }

            if constexpr (words_needed == 1) {
                const std::uint32_t v = this->m_cache[this->m_index];
                this->m_index += 1;
                return static_cast<T>(v);
            } else {
                const std::uint64_t lo = this->m_cache[this->m_index];
                const std::uint64_t hi = this->m_cache[this->m_index + 1];
                this->m_index += 2;
                return static_cast<T>(lo | (hi << 32));
            }
        }

        /**
         * @brief Generates a uniform random floating-point number in [0, 1).
         *
         * @tparam T Floating-point type (float, double, bfloat16, or half).
         * @return Uniform random value in range [0, 1).
         */
        template<float_like T>
        [[nodiscard]]
        TLX_HD T uniform() noexcept {
            if constexpr (std::same_as<T, double>) {
                constexpr double norm = 1.0 / static_cast<double>(std::uint64_t{1} << 53);
                const std::uint64_t bits = next<std::uint64_t>() >> 11;
                return static_cast<double>(bits) * norm;
            } else {
                constexpr float norm = 1.0f / static_cast<float>(std::uint32_t{1} << 24);
                const std::uint32_t bits = next<std::uint32_t>() >> 8;
                return static_cast<T>(static_cast<float>(bits) * norm);
            }
        }

        /**
         * @brief Generates a uniform random floating-point number in [min, max).
         *
         * @tparam T Floating-point type.
         * @param min Lower bound (inclusive).
         * @param max Upper bound (exclusive).
         * @return Uniform random value in range [min, max).
         */
        template<float_like T>
        [[nodiscard]]
        TLX_HD T uniform(T min, T max) noexcept {
            const T u = uniform<T>();
            return min + u * (max - min);
        }

        template<float_like T>
        [[nodiscard]]
        TLX_HD T normal() noexcept {
            if constexpr (std::same_as<T, double>) {
                constexpr double two_pi = 6.283185307179586476925286766559;
                auto u1 = uniform<double>();
                if (u1 < 1e-300) {
                    u1 = 1e-300;
                }
                const auto u2 = uniform<double>();
                const double r = sqrt(-2.0 * log(u1));
                const double theta = two_pi * u2;
                return static_cast<T>(r * cos(theta));
            } else {
                constexpr float two_pi = 6.283185307179586f;
                auto u1 = uniform<float>();
                if (u1 < 1e-7f) {
                    u1 = 1e-7f;
                }
                const auto u2 = uniform<float>();
                const float r = sqrtf(-2.0f * logf(u1));
                const float theta = two_pi * u2;
                return static_cast<T>(r * cosf(theta));
            }
        }

        template<float_like T>
        [[nodiscard]]
        TLX_HD T normal(const T mean, const T stddev) noexcept {
            return mean + stddev * normal<T>();
        }

    private:
        struct counter {
            std::uint32_t x, y, z, w;
        };

        struct key {
            std::uint32_t k0, k1;
        };

        counter m_counter{};
        key m_key{};

        std::uint32_t m_cache[4]{};
        std::uint32_t m_index = 4;

        [[nodiscard]]
        TLX_HD static  counter generate(counter ctr, key key) noexcept {
            for (int round = 0; round < 10; ++round) {

                constexpr std::uint32_t BUMP1 = 0xBB67AE85u;
                constexpr std::uint32_t BUMP0 = 0x9E3779B9u;
                constexpr std::uint32_t MULT1 = 0xCD9E8D57u;
                constexpr std::uint32_t MULT0 = 0xD2511F53u;

                const std::uint32_t hi0 = mul_hi(MULT0, ctr.x);
                const std::uint32_t lo0 = mul_lo(MULT0, ctr.x);
                const std::uint32_t hi1 = mul_hi(MULT1, ctr.z);
                const std::uint32_t lo1 = mul_lo(MULT1, ctr.z);

                ctr = counter{
                    hi1 ^ ctr.y ^ key.k0,
                    lo1,
                    hi0 ^ ctr.w ^ key.k1,
                    lo0
                };

                key.k0 += BUMP0;
                key.k1 += BUMP1;
            }
            return ctr;
        }

        TLX_HD void refill() noexcept {
            const auto [x, y, z, w] = generate(this->m_counter, this->m_key);
            this->m_cache[0] = x;
            this->m_cache[1] = y;
            this->m_cache[2] = z;
            this->m_cache[3] = w;
            this->m_index = 0;
            increment_counter();
        }

        [[nodiscard]]
        TLX_HD static constexpr std::uint32_t mul_hi(const std::uint32_t a, const std::uint32_t b) noexcept {
            const std::uint64_t product = static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);
            return static_cast<std::uint32_t>(product >> 32);
        }

        [[nodiscard]]
        TLX_HD static constexpr std::uint32_t mul_lo(const std::uint32_t a, const std::uint32_t b) noexcept {
            return a * b;
        }

        TLX_HD void increment_counter() noexcept {
            if (++this->m_counter.x == 0) {
                if (++this->m_counter.y == 0) {
                    if (++this->m_counter.z == 0) {
                        ++this->m_counter.w;
                    }
                }
            }
        }
    };
} //namespace tlx

#endif //TLX_RANDOM_HPP