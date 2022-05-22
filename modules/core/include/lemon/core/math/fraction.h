#pragma once

#include <lemon/core/basic_types.h>

namespace lemon {
template<class Rep = size_type>
struct fraction
{
    Rep num{}, den{};

    operator Rep() { return Rep(num) / den; }
    operator f32() { return f32(num) / den; }
    operator f64() { return f64(num) / den; }

    fraction& operator+=(const fraction& other)
    {
        num = num * other.den + other.num * den;
        den *= other.den;
        return *this;
    }
    fraction& operator-=(const fraction& other)
    {
        num = num * other.den - other.num * den;
        den *= other.den;
        return *this;
    }
    fraction& operator*=(const fraction& other)
    {
        num *= other.num;
        den *= other.den;
        return *this;
    }
    fraction& operator/=(const fraction& other)
    {
        num *= other.den;
        den *= other.num;
    }
};
inline fraction operator+(const fraction& lhs, const fraction& rhs)
{
    return fraction(lhs) += rhs;
}
inline fraction operator-(const fraction& lhs, const fraction& rhs)
{
    return fraction(lhs) -= rhs;
}
inline fraction operator*(const fraction& lhs, const fraction& rhs)
{
    return fraction(lhs) *= rhs;
}
inline fraction operator/(const fraction& lhs, const fraction& rhs)
{
    return fraction(lhs) /= rhs;
}
} // namespace lemon
