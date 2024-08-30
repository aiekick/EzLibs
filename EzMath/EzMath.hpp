#pragma once

#include <string>
#include <sstream>
#include <iomanip>  // std::setprecision
#include <cmath>

namespace ez {

inline std::string round_n(double vvalue, int n) {
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

inline bool isFloatDifferent(float vA, float vB) {
    return (fabs(vA - vB) > FLT_EPSILON);
}

inline bool isFloatEqual(float vA, float vB) {
    return (fabs(vA - vB) < FLT_EPSILON);
}

template <typename T>
inline T round(const T& v) {
    return (T)(std::round((double)v));
}
template <typename T>
inline T floor(const T& v) {
    return (T)(std::floor((double)v));
}
template <typename T>
inline T ceil(const T& v) {
    return (T)(std::ceil((double)v));
}
template <typename T>
inline T fract(const T& v) {
    return v - floor<T>(v);
}
template <typename T>
inline T cos(const T& v) {
    return std::cos(v);
}
template <typename T>
inline T acos(const T& v) {
    return std::acos(v);
}
template <typename T>
inline T sin(const T& v) {
    return std::sin(v);
}
template <typename T>
inline T asin(const T& v) {
    return std::asin(v);
}
template <typename T>
inline T tan(const T& v) {
    return std::tan(v);
}
template <typename T>
inline T atan(const T& v) {
    return std::atan(v);
}
template <typename T>
inline T mini(const T& a, const T& b) {
    return a < b ? a : b;
}
template <typename T>
inline T maxi(const T& a, const T& b) {
    return a > b ? a : b;
}

// for use in internals types, like vec2, vec3, vec4
template <typename T>
inline T internal_mini(const T& a, const T& b) {
    return a < b ? a : b;
}
template <typename T>
inline T internal_maxi(const T& a, const T& b) {
    return a > b ? a : b;
}

// template <typename T> inline T clamp(const T& a, const T& b, const T& c) { return mini<const T&>(maxi<const T&>(a, b), c); }

template <typename T>
inline T clamp(const T& n) {
    return n >= T(0) && n <= T(1) ? n : T(n > T(0));
}  // clamp n => 0 to 1
template <typename T>
inline T clamp(const T& n, const T& b) {
    return n >= T(0) && n <= b ? n : T(n > T(0)) * b;
}  // clamp n => 0 to b
template <typename T>
inline T clamp(const T& n, const T& a, const T& b) {
    return n >= a && n <= b ? n : n < a ? a : b;
}  // clamp n => a to b

template <typename T>
inline T abs(const T& a) {
    return a < 0 ? a * (T)-1 : a;
}
template <typename T>
inline T sign(const T& a) {
    return a < 0 ? (T)-1 : (T)1;
}
template <typename T>
inline T step(const T& a, const T& b) {
    return b < a ? (T)0 : (T)1;
}
template <typename T>
inline T mod(const T& v, const T& l) {
    return std::modf(v, l);
}
template <typename T>
inline T invMix(const T& i, const T& s, const T& r) {
    return (r - i) / (s - i);
}

// https://twitter.com/FreyaHolmer/status/1068293398073929728
template <typename T>
inline T lerp(const T& a, const T& b, const T& t) {
    return a * (1.0f - t) + b * t;
}
template <typename T>
inline T eerp(const T& a, const T& b, const T& t) {
    if (a == (T)0)
        return T(0);
    return pow(a * (b / a), t);
}

template <typename T>
inline T mix(const T& a, const T& b, const T& t) {
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

