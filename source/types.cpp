//
// Created by muham on 15.07.2026.
//

#include "tlx/types.hpp"
#include <iostream>

namespace tlx {
    std::ostream& operator<<(std::ostream &os, const bfloat16 &rhs) {
        os << static_cast<float>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const half& rhs) {
        os << static_cast<float>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream &os, const qint8 &rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const qint16& rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const quint8& rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const quint16& rhs) {
        os << static_cast<std::int32_t>(rhs);
        return os;
    }
} //namespace tlx

using namespace tlx;

vec8f::vec8f() : reg(_mm256_setzero_ps()) {}

vec8f::vec8f(const float *x) : reg(_mm256_setzero_ps()) {
    this->reg = _mm256_loadu_ps(x);
}

vec8f::vec8f(const __m256 x) : reg(_mm256_setzero_ps()) {
    this->reg = x;
}

void vec8f::store(float *x) const {
    _mm256_storeu_ps(x, this->reg);
}

vec8f::operator __m256() const {
    return this->reg;
}

vec8f vec8f::operator+(const vec8f &other) const {
    return _mm256_add_ps(this->reg, other.reg);
}

vec8f vec8f::operator-(const vec8f &other) const {
    return _mm256_sub_ps(this->reg, other.reg);
}

vec8f vec8f::operator*(const vec8f &other) const {
    return _mm256_mul_ps(this->reg, other.reg);
}

vec8f vec8f::operator/(const vec8f &other) const {
    return _mm256_div_ps(this->reg, other.reg);
}

vec8f& vec8f::operator+=(const vec8f &other) {
    this->reg = _mm256_add_ps(this->reg, other.reg);
    return *this;
}

vec8f &vec8f::operator-=(const vec8f &other) {
    this->reg = _mm256_sub_ps(this->reg, other.reg);
    return *this;
}

vec8f &vec8f::operator*=(const vec8f &other) {
    this->reg = _mm256_mul_ps(this->reg, other.reg);
    return *this;
}

vec8f &vec8f::operator/=(const vec8f &other) {
    this->reg = _mm256_div_ps(this->reg, other.reg);
    return *this;
}

vec8f vec8f::operator==(const vec8f &other) const {
    return _mm256_cmp_ps(this->reg, other.reg, _CMP_EQ_OQ);
}

vec8f vec8f::operator!=(const vec8f &other) const {
    return _mm256_cmp_ps(this->reg, other.reg, _CMP_NEQ_OQ);
}

vec8f vec8f::operator<(const vec8f &other) const {
    return _mm256_cmp_ps(this->reg, other.reg, _CMP_LT_OQ);
}

vec8f vec8f::operator<=(const vec8f &other) const {
    return _mm256_cmp_ps(this->reg, other.reg, _CMP_LE_OQ);
}

vec8f vec8f::operator>(const vec8f &other) const {
    return _mm256_cmp_ps(this->reg, other.reg, _CMP_GT_OQ);
}

vec8f vec8f::operator>=(const vec8f &other) const {
    return _mm256_cmp_ps(this->reg, other.reg, _CMP_GE_OQ);
}

vec8i::vec8i(const std::int32_t *x) : reg(_mm256_setzero_si256()) {
    this->reg = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(x));
}

vec8i::vec8i(const __m256i x) : reg(_mm256_setzero_si256()) {
    this->reg = x;
}

void vec8i::store(std::int32_t *x) const {
    _mm256_storeu_si256(reinterpret_cast<__m256i *>(x), this->reg);
}

vec8i::operator __m256i() const {
    return this->reg;
}

vec8i vec8i::operator+(const vec8i &other) const {
    return _mm256_add_epi32(this->reg, other.reg);
}

vec8i vec8i::operator-(const vec8i &other) const {
    return _mm256_sub_epi32(this->reg, other.reg);
}

vec8i vec8i::operator*(const vec8i &other) const {
    return _mm256_mullo_epi32(this->reg, other.reg);
}

vec8i vec8i::operator/(const vec8i &other) const {
    return _mm256_div_epi32(this->reg, other.reg);
}

vec8i &vec8i::operator+=(const vec8i &other) {
    this->reg = _mm256_add_epi32(this->reg, other.reg);
    return *this;
}

vec8i &vec8i::operator-=(const vec8i &other) {
    this->reg = _mm256_sub_epi32(this->reg, other.reg);
    return *this;
}

vec8i &vec8i::operator*=(const vec8i &other) {
    this->reg = _mm256_mul_epi32(this->reg, other.reg);
    return *this;
}

vec8i &vec8i::operator/=(const vec8i &other) {
    this->reg = _mm256_div_epi32(this->reg, other.reg);
    return *this;
}

vec8i vec8i::operator==(const vec8i &other) const {
    return _mm256_cmpeq_epi32(this->reg, other.reg);
}

vec8i vec8i::operator!=(const vec8i &other) const {
    const __m256i eq = _mm256_cmpeq_epi32(this->reg, other.reg);
    return _mm256_xor_si256(eq, _mm256_set1_epi32(-1));
}

vec8i vec8i::operator>(const vec8i &other) const {
    return _mm256_cmpgt_epi32(this->reg, other.reg);
}

vec8i vec8i::operator<(const vec8i &other) const {
    return _mm256_cmpgt_epi32(other.reg, this->reg);
}

vec8i vec8i::operator>=(const vec8i &other) const {
    const __m256i lt = _mm256_cmpgt_epi32(other.reg, this->reg);
    return _mm256_xor_si256(lt, _mm256_set1_epi32(-1));
}

vec8i vec8i::operator<=(const vec8i &other) const {
    const __m256i gt = _mm256_cmpgt_epi32(this->reg, other.reg);
    return _mm256_xor_si256(gt, _mm256_set1_epi32(-1));
}

vec4d::vec4d(const double *x) : reg(_mm256_setzero_pd()) {
    this->reg = _mm256_loadu_pd(x);
}

vec4d::vec4d(const __m256d x) : reg(_mm256_setzero_pd()) {
    this->reg = x;
}

void vec4d::store(double *x) const {
    _mm256_storeu_pd(x, this->reg);
}

vec4d::operator __m256d() const {
    return this->reg;
}

vec4d vec4d::operator+(const vec4d &other) const {
    return _mm256_add_pd(this->reg, other.reg);
}

vec4d vec4d::operator-(const vec4d &other) const {
    return _mm256_sub_pd(this->reg, other.reg);
}

vec4d vec4d::operator*(const vec4d &other) const {
    return _mm256_mul_pd(this->reg, other.reg);
}

vec4d vec4d::operator/(const vec4d &other) const {
    return _mm256_div_pd(this->reg, other.reg);
}

vec4d &vec4d::operator+=(const vec4d &other) {
    this->reg = _mm256_add_pd(this->reg, other.reg);
    return *this;
}

vec4d &vec4d::operator-=(const vec4d &other) {
    this->reg = _mm256_sub_pd(this->reg, other.reg);
    return *this;
}

vec4d &vec4d::operator*=(const vec4d &other) {
    this->reg = _mm256_mul_pd(this->reg, other.reg);
    return *this;
}

vec4d &vec4d::operator/=(const vec4d &other) {
    this->reg = _mm256_div_pd(this->reg, other.reg);
    return *this;
}

vec4d vec4d::operator==(const vec4d &other) const {
    return _mm256_cmp_pd(this->reg, other.reg, _CMP_EQ_OQ);
}

vec4d vec4d::operator!=(const vec4d &other) const {
    return _mm256_cmp_pd(this->reg, other.reg, _CMP_NEQ_OQ);
}

vec4d vec4d::operator<(const vec4d &other) const {
    return _mm256_cmp_pd(this->reg, other.reg, _CMP_LT_OQ);
}

vec4d vec4d::operator<=(const vec4d &other) const {
    return _mm256_cmp_pd(this->reg, other.reg, _CMP_LE_OQ);
}

vec4d vec4d::operator>(const vec4d &other) const {
    return _mm256_cmp_pd(this->reg, other.reg, _CMP_GT_OQ);
}

vec4d vec4d::operator>=(const vec4d &other) const {
    return _mm256_cmp_pd(this->reg, other.reg, _CMP_GE_OQ);
}