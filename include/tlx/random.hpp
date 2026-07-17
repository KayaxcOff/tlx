//
// Created by muham on 17.07.2026.
//

#ifndef TLX_RANDOM_HPP
#define TLX_RANDOM_HPP

#include <tlx/macros.hpp>
#include <limits>

namespace tlx {
    class xoroshiro128_plus {
    public:
        TLX_HD explicit xoroshiro128_plus(std::uint64_t seed) noexcept {
            this->m_s0 = splitmix64(seed);
            this->m_s1 = splitmix64(seed);
        }

        [[nodiscard]]
        TLX_HD std::uint64_t next() {
            const std::uint64_t s0 = this->m_s0;
            std::uint64_t s1 = this->m_s1;

            const std::uint64_t output = s0 + s1;

            s1 ^= s0;

            this->m_s0 = rotl(s0, 55) ^ s1 ^ (s1 << 14);
            this->m_s1 = rotl(s1, 36);

            return output;
        }

        [[nodiscard]]
        TLX_HD static constexpr std::uint64_t rotl(const std::uint64_t x, const int k) {
            return (x << k) | (x >> (64 - k));
        }

        template<typename T>
        [[nodiscard]]
        TLX_HD T next() {
            return static_cast<T>(next());
        }

        TLX_HD std::uint64_t operator()() noexcept {
            return next();
        }
        TLX_HD static constexpr std::uint64_t min() noexcept {
            return std::numeric_limits<std::uint64_t>::min();
        }
        TLX_HD static constexpr std::uint64_t max() noexcept {
            return std::numeric_limits<std::uint64_t>::max();
        }

        template<std::floating_point T>
        TLX_HD T uniform() noexcept;

        template<>
        [[nodiscard]]
        TLX_HD float xoroshiro128_plus::uniform<float>() noexcept {
            constexpr float inv = 1.0f / 16777216.0f;

            return static_cast<float>(next() >> 40) * inv;
        }

    private:
        std::uint64_t m_s0;
        std::uint64_t m_s1;

        [[nodiscard]]
        TLX_HD static std::uint64_t splitmix64(std::uint64_t& state) {
            std::uint64_t z = (state += 0x9e3779b97f4a7c15ULL);

            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
            z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;

            return z ^ (z >> 31);
        }
    };
} //namespace tlx

#endif //TLX_RANDOM_HPP