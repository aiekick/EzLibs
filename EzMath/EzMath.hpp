#pragma once

#include <type_traits>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <limits>

namespace ez {

// This function rounds a floating-point number to 'n' decimal places.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, std::string>::type round_n(T vvalue, int n) {
    std::stringstream tmp;
    tmp << std::setprecision(n) << std::fixed << vvalue;
    return tmp.str();
}

/// This function is used to ensure that a floating point number is not a NaN or infinity.
inline bool floatIsValid(float x) {
    const union {
        float f;
        int32_t i;
    } v = {x};
    return (v.i & 0x7f800000) != 0x7f800000;
}

// Checks if two floating-point numbers are different according to epsilon precision.
inline bool isFloatDifferent(float vA, float vB) {
    return std::fabs(vA - vB) > FLT_EPSILON;
}

// Checks if two floating-point numbers are equal according to epsilon precision.
inline bool isFloatEqual(float vA, float vB) {
    return std::fabs(vA - vB) < FLT_EPSILON;
}

// Rounds a floating-point number to the nearest integer.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type round(T v) {
    return static_cast<T>(std::round(v));
}

// Returns the largest integer less than or equal to the floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type floor(T v) {
    return static_cast<T>(std::floor(v));
}

// Returns the smallest integer greater than or equal to the floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type ceil(T v) {
    return static_cast<T>(std::ceil(v));
}

// Returns the fractional part of a floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type fract(T v) {
    return v - floor(v);
}

// Computes the cosine of a floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type cos(T v) {
    return std::cos(v);
}

// Computes the arc cosine of a floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type acos(T v) {
    return std::acos(v);
}

// Computes the sine of a floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type sin(T v) {
    return std::sin(v);
}

// Computes the arc sine of a floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type asin(T v) {
    return std::asin(v);
}

// Computes the tangent of a floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type tan(T v) {
    return std::tan(v);
}

// Computes the arc tangent of a floating-point number.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type atan(T v) {
    return std::atan(v);
}

// Returns the smaller of two values.
// Works with both integral and floating-point types.
template <typename T>
inline T mini(T a, T b) {
    return a < b ? a : b;
}

// Returns the larger of two values.
// Works with both integral and floating-point types.
template <typename T>
inline T maxi(T a, T b) {
    return a > b ? a : b;
}

// Clamps a value between 0 and 1.
// Works with both integral and floating-point types.
template <typename T>
inline T clamp(T n) {
    return n >= static_cast<T>(0) && n <= static_cast<T>(1) ? n : static_cast<T>(n > static_cast<T>(0));
}

// Clamps a value between 0 and b.
// Works with both integral and floating-point types.
template <typename T>
inline T clamp(T n, T b) {
    return n >= static_cast<T>(0) && n <= b ? n : static_cast<T>(n > static_cast<T>(0)) * b;
}

// Clamps a value between a and b.
// Works with both integral and floating-point types.
template <typename T>
inline T clamp(T n, T a, T b) {
    return n >= a && n <= b ? n : n < a ? a : b;
}

// Computes the absolute value of a number.
// Works with both integral and floating-point types.
template <typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type abs(T a) {
    return a < 0 ? a * static_cast<T>(-1) : a;
}

// Determines the sign of a number (-1 for negative, 1 for positive).
// Works with both integral and floating-point types.
template <typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type sign(T a) {
    return a < 0 ? static_cast<T>(-1) : static_cast<T>(1);
}

// Returns 0 if b < a, otherwise returns 1.
// Works with both integral and floating-point types.
template <typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type step(T a, T b) {
    return b < a ? static_cast<T>(0) : static_cast<T>(1);
}

// Computes the floating-point remainder of the division operation.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type mod(T v, T l) {
    return std::fmod(v, l);
}

// Computes the inverse of the linear interpolation mix.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type invMix(T i, T s, T r) {
    return (r - i) / (s - i);
}

// Linearly interpolates between a and b by t.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type lerp(T a, T b, T t) {
    return a * (static_cast<T>(1.0) - t) + b * t;
}

// Exponentially interpolates between a and b by t.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type eerp(T a, T b, T t) {
    if (a == static_cast<T>(0))
        return static_cast<T>(0);
    return std::pow(a * (b / a), t);
}

// Performs linear interpolation (lerp) between a and b by t.
// Only floating-point types (float, double, long double) are allowed.
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, T>::type mix(T a, T b, T t) {
    return lerp(a, b, t);
}

}  // namespace ez

#include "EzVec2.hpp"
#include "EzVec3.hpp"
#include "EzVec4.hpp"
#include "EzAABB.hpp"
#include "EzQuat.hpp"
#include "EzPlane.hpp"
#include "EzAABBCC.hpp"
#include "EzVariant.hpp"

