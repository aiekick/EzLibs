#include <TestEzMath.h>
#include <EzMath/EzMath.hpp>
#include <cmath>
#include <limits>

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Désactivation des warnings de conversion
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)  // Conversion from 'double' to 'float', possible loss of data
#pragma warning(disable : 4305)  // Truncation from 'double' to 'float'
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int TestEzMathRoundN() {
    if (ez::round_n(1.23456, 2) != "1.23")
        return false;
    if (ez::round_n(1.23456, 4) != "1.2346")
        return false;
    if (ez::round_n(1.2, 1) != "1.2")
        return false;
    return true;
}

int TestEzMathFloatIsValid() {
    if (!ez::floatIsValid(1.0f))
        return false;
    if (!ez::floatIsValid(0.0f))
        return false;
    if (ez::floatIsValid(std::numeric_limits<float>::infinity()))
        return false;
    if (ez::floatIsValid(std::numeric_limits<float>::quiet_NaN()))
        return false;
    return true;
}

int TestEzMathIsFloatDifferent() {
    if (ez::isFloatDifferent(1.0f, 1.0f))
        return false;
    if (ez::isFloatDifferent(1.0f, 1.0f + FLT_EPSILON))
        return false;
    if (!ez::isFloatDifferent(1.0f, 1.0f + 2.0f * FLT_EPSILON))
        return false;
    return true;
}

int TestEzMathIsFloatEqual() {
    if (!ez::isFloatEqual(1.0f, 1.0f))
        return false;
    if (ez::isFloatEqual(1.0f, 1.0f + 2.0f * FLT_EPSILON))
        return false;
    return true;
}

template <typename T>
int TestEzMathRound() {
    if (!ez::isFloatEqual(ez::round<T>(0.5), 1.0))
        return false;
    if (!ez::isFloatEqual(ez::round<T>(0.0), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::round<T>(1.2), 1.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathFloor() {
    if (!ez::isFloatEqual(ez::floor<T>(0.5), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::floor<T>(0.0), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::floor<T>(1.2), 1.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathCeil() {
    if (!ez::isFloatEqual(ez::ceil<T>(0.5), 1.0))
        return false;
    if (!ez::isFloatEqual(ez::ceil<T>(0.0), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::ceil<T>(1.2), 2.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathFract() {
    if (!ez::isFloatEqual(ez::fract<T>(0.5), 0.5))
        return false;
    if (!ez::isFloatEqual(ez::fract<T>(1.2), 0.2))
        return false;
    if (!ez::isFloatEqual(ez::fract<T>(2.0), 0.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathCos() {
    if (!ez::isFloatEqual(ez::cos<T>(0.0), 1.0))
        return false;
    if (!ez::isFloatEqual(ez::cos<T>(M_PI / 2.0), 0.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathAcos() {
    if (!ez::isFloatEqual(ez::acos<T>(1.0), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::acos<T>(0.0), M_PI / 2.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathSin() {
    if (!ez::isFloatEqual(ez::sin<T>(0.0), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::sin<T>(M_PI / 2.0), 1.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathAsin() {
    if (!ez::isFloatEqual(ez::asin<T>(0.0), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::asin<T>(1.0), M_PI / 2.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathTan() {
    if (!ez::isFloatEqual(ez::tan<T>(0.0), 0.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathAtan() {
    if (!ez::isFloatEqual(ez::atan<T>(0.0), 0.0))
        return false;
    if (!ez::isFloatEqual(ez::atan<T>(1.0), M_PI / 4.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathMini() {
    if (ez::mini<T>(1, 2) != 1)
        return false;
    if (ez::mini<T>(2, 1) != 1)
        return false;
    return true;
}

template <typename T>
int TestEzMathMaxi() {
    if (ez::maxi<T>(1, 2) != 2)
        return false;
    if (ez::maxi<T>(2, 1) != 2)
        return false;
    return true;
}

template <typename T>
int TestEzMathClamp() {
    if (ez::clamp<T>(-1, 0, 1) != 0)
        return false;
    if (ez::clamp<T>(2, 0, 1) != 1)
        return false;
    if (!ez::isFloatEqual(ez::clamp<T>(0.5, 0, 1), 0.5))
        return false;
    return true;
}

// Specialisation for int32_t, only positive values
template <>
int TestEzMathClamp<int32_t>() {
    if (ez::clamp<int32_t>(-1, 0, 1) != 0)
        return false;
    if (ez::clamp<int32_t>(-2, -5, 5) != -2)
        return false;
    if (ez::clamp<int32_t>(2, -5, -2) != -2)
        return false;
    return true;
}

// Specialisation for uint32_t, only positive values
template <>
int TestEzMathClamp<uint32_t>() {
    if (ez::clamp<uint32_t>(2, 0, 10) != 2)
        return false;
    if (ez::clamp<uint32_t>(2, 0, 1) != 1)
        return false;
    if (ez::clamp<uint32_t>(2, 5, 10) != 5)
        return false;
    return true;
}

template <typename T>
int TestEzMathAbs() {
    if (ez::abs<T>(-1) != 1)
        return false;
    if (ez::abs<T>(1) != 1)
        return false;
    return true;
}

template <typename T>
int TestEzMathSign() {
    if (ez::sign<T>(-1) != -1)
        return false;
    if (ez::sign<T>(1) != 1)
        return false;
    return true;
}

template <typename T>
int TestEzMathStep() {
    if (ez::step<T>(0.5, 0.4) != 0)
        return false;
    if (ez::step<T>(0.5, 0.6) != 1)
        return false;
    return true;
}

template <typename T>
int TestEzMathMod() {
    if (!ez::isFloatEqual(ez::mod<T>(5.5, 2.0), 1.5))
        return false;
    if (!ez::isFloatEqual(ez::mod<T>(4.0, 2.0), 0.0))
        return false;
    return true;
}

template <typename T>
int TestEzMathInvMix() {
    if (!ez::isFloatEqual(ez::invMix<T>(1, 2, 1.5), 0.5))
        return false;
    return true;
}

template <typename T>
int TestEzMathLerp() {
    if (!ez::isFloatEqual(ez::lerp<T>(0, 1, 0.5), 0.5))
        return false;
    if (!ez::isFloatEqual(ez::lerp<T>(1, 2, 0.5), 1.5))
        return false;
    return true;
}

template <typename T>
int TestEzMathEerp() {
    if (!ez::isFloatEqual(ez::eerp<T>(1, 2, 0.5), sqrt(2.0)))
        return false;
    return true;
}

template <typename T>
int TestEzMathMix() {
    if (!ez::isFloatEqual(ez::mix<T>(0, 1, 0.5), 0.5))
        return false;
    if (!ez::isFloatEqual(ez::mix<T>(1, 2, 0.5), 1.5))
        return false;
    return true;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

bool TestEzMath(const std::string& vTest) {
    IfTestExist(TestEzMathRoundN);

    else IfTestExist(TestEzMathFloatIsValid);

    else IfTestExist(TestEzMathIsFloatDifferent);

    else IfTestExist(TestEzMathIsFloatEqual);

    else IfTestExist(TestEzMathRound<float>);
    else IfTestExist(TestEzMathRound<double>);

    else IfTestExist(TestEzMathFloor<float>);
    else IfTestExist(TestEzMathFloor<double>);

    else IfTestExist(TestEzMathCeil<float>);
    else IfTestExist(TestEzMathCeil<double>);

    else IfTestExist(TestEzMathFract<float>);
    else IfTestExist(TestEzMathFract<double>);

    else IfTestExist(TestEzMathCos<float>);
    else IfTestExist(TestEzMathCos<double>);

    else IfTestExist(TestEzMathAcos<float>);
    else IfTestExist(TestEzMathAcos<double>);

    else IfTestExist(TestEzMathSin<float>);
    else IfTestExist(TestEzMathSin<double>);

    else IfTestExist(TestEzMathAsin<float>);
    else IfTestExist(TestEzMathAsin<double>);

    else IfTestExist(TestEzMathTan<float>);
    else IfTestExist(TestEzMathTan<double>);

    else IfTestExist(TestEzMathAtan<float>);
    else IfTestExist(TestEzMathAtan<double>);

    else IfTestExist(TestEzMathMini<int32_t>);
    else IfTestExist(TestEzMathMini<uint32_t>);
    else IfTestExist(TestEzMathMini<float>);
    else IfTestExist(TestEzMathMini<double>);

    else IfTestExist(TestEzMathMaxi<int32_t>);
    else IfTestExist(TestEzMathMaxi<uint32_t>);
    else IfTestExist(TestEzMathMaxi<float>);
    else IfTestExist(TestEzMathMaxi<double>);

    else IfTestExist(TestEzMathClamp<int32_t>);
    else IfTestExist(TestEzMathClamp<uint32_t>);
    else IfTestExist(TestEzMathClamp<float>);
    else IfTestExist(TestEzMathClamp<double>);

    else IfTestExist(TestEzMathAbs<int32_t>);
    else IfTestExist(TestEzMathAbs<float>);
    else IfTestExist(TestEzMathAbs<double>);

    else IfTestExist(TestEzMathSign<int32_t>);
    else IfTestExist(TestEzMathSign<float>);
    else IfTestExist(TestEzMathSign<double>);

    else IfTestExist(TestEzMathStep<float>);
    else IfTestExist(TestEzMathStep<double>);

    else IfTestExist(TestEzMathMod<float>);
    else IfTestExist(TestEzMathMod<double>);

    else IfTestExist(TestEzMathInvMix<float>);
    else IfTestExist(TestEzMathInvMix<double>);

    else IfTestExist(TestEzMathLerp<float>);
    else IfTestExist(TestEzMathLerp<double>);

    else IfTestExist(TestEzMathEerp<float>);
    else IfTestExist(TestEzMathEerp<double>);

    else IfTestExist(TestEzMathMix<float>);
    else IfTestExist(TestEzMathMix<double>);

    return false;
}

#ifdef _MSC_VER
#pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
