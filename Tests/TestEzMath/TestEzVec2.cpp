#include <EzMath/EzMath.hpp>
#include <cmath>
#include <limits>

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

template <typename T>
bool TestEzVec2Offset() {
    ez::vec2<T> v(1, 2);
    ez::vec2<T> result = v.Offset(1, 2);
    if (result.x != 2 || result.y != 4)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Negate() {
    ez::vec2<T> v(1, 2);
    ez::vec2<T> result = -v;
    if (result.x != -1 || result.y != -2)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Length() {
    ez::vec2<T> v(3, 4);
    if (std::abs(v.length() - 5) >= std::numeric_limits<T>::epsilon())
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Normalize() {
    ez::vec2<T> v(3, 4);
    v.normalize();
    if (std::abs(v.length() - 1) >= std::numeric_limits<T>::epsilon())
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Sum() {
    ez::vec2<T> v(1, 2);
    if (v.sum() != static_cast<T>(3))
        return false;
    return true;
}

template <typename T>
bool TestEzVec2OperatorAdd() {
    ez::vec2<T> v1(1, 2);
    ez::vec2<T> v2(3, 4);
    T scalar = 1;

    ez::vec2<T> result1 = v1 + scalar;
    if (result1.x != 2 || result1.y != 3)
        return false;

    ez::vec2<T> result2 = scalar + v1;
    if (result2.x != 2 || result2.y != 3)
        return false;

    ez::vec2<T> result3 = v1 + v2;
    if (result3.x != 4 || result3.y != 6)
        return false;

    return true;
}

template <typename T>
bool TestEzVec2OperatorSubtract() {
    ez::vec2<T> v1(3, 4);
    ez::vec2<T> v2(1, 2);
    T scalar = 1;

    ez::vec2<T> result1 = v1 - scalar;
    if (result1.x != 2 || result1.y != 3)
        return false;

    ez::vec2<T> result2 = scalar - v1;
    if (result2.x != -2 || result2.y != -3)
        return false;

    ez::vec2<T> result3 = v1 - v2;
    if (result3.x != 2 || result3.y != 2)
        return false;

    return true;
}

template <typename T>
bool TestEzVec2OperatorMultiply() {
    ez::vec2<T> v1(2, 3);
    ez::vec2<T> v2(4, 5);
    T scalar = 2;

    ez::vec2<T> result1 = v1 * scalar;
    if (result1.x != 4 || result1.y != 6)
        return false;

    ez::vec2<T> result2 = scalar * v1;
    if (result2.x != 4 || result2.y != 6)
        return false;

    ez::vec2<T> result3 = v1 * v2;
    if (result3.x != 8 || result3.y != 15)
        return false;

    return true;
}

template <typename T>
bool TestEzVec2OperatorDivide() {
    ez::vec2<T> v1(4, 6);
    ez::vec2<T> v2(2, 3);
    T scalar = 2;

    ez::vec2<T> result1 = v1 / scalar;
    if (result1.x != 2 || result1.y != 3)
        return false;

    ez::vec2<T> result3 = v1 / v2;
    if (result3.x != 2 || result3.y != 2)
        return false;

    return true;
}

template <>
bool TestEzVec2OperatorDivide<float>() {
    ez::vec2<float> v1(4.0f, 6.0f);
    ez::vec2<float> v2(2.0f, 3.0f);
    float scalar = 2.0f;

    ez::vec2<float> result2 = scalar / v1;
    if (result2.x != 0.5f || ez::isDifferent(result2.y, 1.0f / 3.0f))
        return false;

    return true;
}

template <>
bool TestEzVec2OperatorDivide<double>() {
    ez::vec2<double> v1(4.0, 6.0);
    ez::vec2<double> v2(2.0, 3.0);
    double scalar = 2.0;

    ez::vec2<double> result2 = scalar / v1;
    if (result2.x != 0.5 || ez::isDifferent(result2.y, 1.0 / 3.0))
        return false;

    return true;
}

template <typename T>
bool TestEzVec2EmptyAND() {
    ez::vec2<T> v1(0, 0);
    ez::vec2<T> v2(1, 0);
    ez::vec2<T> v3(0, 1);
    ez::vec2<T> v4(1, 1);

    if (!v1.emptyAND() || v2.emptyAND() || v3.emptyAND() || v4.emptyAND())
        return false;

    return true;
}

template <typename T>
bool TestEzVec2EmptyOR() {
    ez::vec2<T> v1(0, 0);
    ez::vec2<T> v2(1, 0);
    ez::vec2<T> v3(0, 1);
    ez::vec2<T> v4(1, 1);

    if (!v1.emptyOR() || !v2.emptyOR() || !v3.emptyOR() || v4.emptyOR())
        return false;

    return true;
}

template <typename T>
bool TestEzVec2Min() {
    ez::vec2<T> v(1, 2);
    if (v.min() != 1)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Max() {
    ez::vec2<T> v(1, 2);
    if (v.max() != 2)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2GetNormalized() {
    ez::vec2<T> v(3, 4);
    ez::vec2<T> normalized = v.GetNormalized();
    if (std::abs(normalized.length() - 1) >= std::numeric_limits<T>::epsilon())
        return false;
    return true;
}

template <typename T>
bool TestEzVec2ComparisonOperators() {
    ez::vec2<T> v1(1, 2);
    ez::vec2<T> v2(3, 4);
    ez::vec2<T> v3(1, 2);
    T scalar = 2;

    if (!(v1 < v2) || v2 < v1 || !(v1 < scalar) || v2 < scalar)
        return false;

    if (v1 > v2 || !(v2 > v1) || v1 > scalar || !(v2 > scalar))
        return false;

    if (!(v1 <= v2) || !(v1 <= v3) || v2 <= v1 || !(v1 <= scalar))
        return false;

    if (v1 >= v2 || !(v2 >= v1) || !(v2 >= scalar))
        return false;

    if (!(v1.operator==(v3)) || v1.operator==(v2))
        return false;

    if (!(v1.operator!=(v2)) || v1.operator!=(v3))
        return false;

    return true;
}

template <typename T>
bool TestEzVec2Floor() {
    ez::vec2<T> v(1.5, 2.7);
    ez::vec2<T> floorResult = ez::floor<T>(v);
    if (floorResult.x != 1 || floorResult.y != 2)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Fract() {
    ez::vec2<T> v(1.5, 2.7);
    ez::vec2<T> fractResult = ez::fract<T>(v);
    if (fractResult.x != 0.5 || fractResult.y != 0.7)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Ceil() {
    ez::vec2<T> v(1.5, 2.7);
    ez::vec2<T> ceilResult = ez::ceil<T>(v);
    if (ceilResult.x != 2 || ceilResult.y != 3)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Mini() {
    ez::vec2<T> v1(1, 3);
    ez::vec2<T> v2(2, 4);
    ez::vec2<T> miniResult = ez::mini<T>(v1, v2);
    if (miniResult.x != 1 || miniResult.y != 3)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Maxi() {
    ez::vec2<T> v1(1, 3);
    ez::vec2<T> v2(2, 4);
    ez::vec2<T> maxiResult = ez::maxi<T>(v1, v2);
    if (maxiResult.x != 2 || maxiResult.y != 4)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Dot() {
    ez::vec2<T> v1(1, 3);
    ez::vec2<T> v2(2, 4);
    T dotResult = ez::dot<T>(v1, v2);
    if (dotResult != 14)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Det() {
    ez::vec2<T> v1(1, 3);
    ez::vec2<T> v2(2, 4);
    T detResult = ez::det<T>(v1, v2);
    if (detResult != -2)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Reflect() {
    ez::vec2<T> v1(1, 3);
    ez::vec2<T> v2(2, 4);
    ez::vec2<T> reflectResult = ez::reflect<T>(v1, v2);
    if (reflectResult.x != -71 || reflectResult.y != -143)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Sign() {
    ez::vec2<T> v(5, 3);
    ez::vec2<T> signResult = ez::sign<T>(v);
    if (signResult.x != 1 || signResult.y != 1)
        return false;

    v = ez::vec2<T>(0, 0);
    signResult = ez::sign(v);
    if (signResult.x != 0 || signResult.y != 0)
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Sin() {
    ez::vec2<T> v(1.5, 2.7);
    ez::vec2<T> sinResult = ez::sin<T>(v);
    if (sinResult.x != std::sin(1.5) || sinResult.y != std::sin(2.7))
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Cos() {
    ez::vec2<T> v(1.5, 2.7);
    ez::vec2<T> cosResult = ez::cos<T>(v);
    if (cosResult.x != std::cos(1.5) || cosResult.y != std::cos(2.7))
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Tan() {
    ez::vec2<T> v(1.5, 2.7);
    ez::vec2<T> tanResult = ez::tan<T>(v);
    if (tanResult.x != std::tan(1.5) || tanResult.y != std::tan(2.7))
        return false;
    return true;
}

template <typename T>
bool TestEzVec2Atan() {
    ez::vec2<T> v(1.5, 2.7);
    ez::vec2<T> atanResult = ez::atan<T>(v);
    if (atanResult.x != std::atan(1.5) || atanResult.y != std::atan(2.7))
        return false;
    return true;
}

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

bool TestEzVec2(const std::string& vTest) {
    IfTestExist(TestEzVec2Offset<float>);
    else IfTestExist(TestEzVec2Offset<double>);
    else IfTestExist(TestEzVec2Offset<int32_t>);
    else IfTestExist(TestEzVec2Offset<int64_t>);
    else IfTestExist(TestEzVec2Offset<uint32_t>);
    else IfTestExist(TestEzVec2Offset<uint64_t>);

    IfTestExist(TestEzVec2Negate<float>);
    else IfTestExist(TestEzVec2Negate<double>);
    else IfTestExist(TestEzVec2Negate<int32_t>);
    else IfTestExist(TestEzVec2Negate<int64_t>);

    IfTestExist(TestEzVec2Length<float>);
    else IfTestExist(TestEzVec2Length<double>);

    IfTestExist(TestEzVec2Normalize<float>);
    else IfTestExist(TestEzVec2Normalize<double>);

    IfTestExist(TestEzVec2Sum<float>);
    else IfTestExist(TestEzVec2Sum<double>);
    else IfTestExist(TestEzVec2Sum<int32_t>);
    else IfTestExist(TestEzVec2Sum<int64_t>);
    else IfTestExist(TestEzVec2Sum<uint32_t>);
    else IfTestExist(TestEzVec2Sum<uint64_t>);

    IfTestExist(TestEzVec2OperatorAdd<float>);
    else IfTestExist(TestEzVec2OperatorAdd<double>);
    else IfTestExist(TestEzVec2OperatorAdd<int32_t>);
    else IfTestExist(TestEzVec2OperatorAdd<int64_t>);
    else IfTestExist(TestEzVec2OperatorAdd<uint32_t>);
    else IfTestExist(TestEzVec2OperatorAdd<uint64_t>);

    IfTestExist(TestEzVec2OperatorSubtract<float>);
    else IfTestExist(TestEzVec2OperatorSubtract<double>);
    else IfTestExist(TestEzVec2OperatorSubtract<int32_t>);
    else IfTestExist(TestEzVec2OperatorSubtract<int64_t>);
    else IfTestExist(TestEzVec2OperatorSubtract<uint32_t>);
    else IfTestExist(TestEzVec2OperatorSubtract<uint64_t>);

    IfTestExist(TestEzVec2OperatorMultiply<float>);
    else IfTestExist(TestEzVec2OperatorMultiply<double>);
    else IfTestExist(TestEzVec2OperatorMultiply<int32_t>);
    else IfTestExist(TestEzVec2OperatorMultiply<int64_t>);
    else IfTestExist(TestEzVec2OperatorMultiply<uint32_t>);
    else IfTestExist(TestEzVec2OperatorMultiply<uint64_t>);

    IfTestExist(TestEzVec2OperatorDivide<float>);
    else IfTestExist(TestEzVec2OperatorDivide<double>);
    else IfTestExist(TestEzVec2OperatorDivide<int32_t>);
    else IfTestExist(TestEzVec2OperatorDivide<int64_t>);
    else IfTestExist(TestEzVec2OperatorDivide<uint32_t>);
    else IfTestExist(TestEzVec2OperatorDivide<uint64_t>);

    IfTestExist(TestEzVec2EmptyAND<float>);
    else IfTestExist(TestEzVec2EmptyAND<double>);
    else IfTestExist(TestEzVec2EmptyAND<int64_t>);
    else IfTestExist(TestEzVec2EmptyAND<uint32_t>);
    else IfTestExist(TestEzVec2EmptyAND<uint64_t>);

    IfTestExist(TestEzVec2EmptyOR<float>);
    else IfTestExist(TestEzVec2EmptyOR<double>);
    else IfTestExist(TestEzVec2EmptyOR<int32_t>);
    else IfTestExist(TestEzVec2EmptyOR<int64_t>);
    else IfTestExist(TestEzVec2EmptyOR<uint32_t>);
    else IfTestExist(TestEzVec2EmptyOR<uint64_t>);

    IfTestExist(TestEzVec2Min<float>);
    else IfTestExist(TestEzVec2Min<double>);
    else IfTestExist(TestEzVec2Min<int32_t>);
    else IfTestExist(TestEzVec2Min<int64_t>);
    else IfTestExist(TestEzVec2Min<uint32_t>);
    else IfTestExist(TestEzVec2Min<uint64_t>);

    IfTestExist(TestEzVec2Max<float>);
    else IfTestExist(TestEzVec2Max<double>);
    else IfTestExist(TestEzVec2Max<int32_t>);
    else IfTestExist(TestEzVec2Max<int64_t>);
    else IfTestExist(TestEzVec2Max<uint32_t>);
    else IfTestExist(TestEzVec2Max<uint64_t>);

    IfTestExist(TestEzVec2GetNormalized<float>);
    else IfTestExist(TestEzVec2GetNormalized<double>);

    IfTestExist(TestEzVec2Floor<float>);
    else IfTestExist(TestEzVec2Floor<double>);

    IfTestExist(TestEzVec2Fract<float>);
    else IfTestExist(TestEzVec2Fract<double>);

    IfTestExist(TestEzVec2Ceil<float>);
    else IfTestExist(TestEzVec2Ceil<double>);

    IfTestExist(TestEzVec2Mini<float>);
    else IfTestExist(TestEzVec2Mini<double>);
    else IfTestExist(TestEzVec2Mini<int32_t>);
    else IfTestExist(TestEzVec2Mini<int64_t>);
    else IfTestExist(TestEzVec2Mini<uint32_t>);
    else IfTestExist(TestEzVec2Mini<uint64_t>);

    IfTestExist(TestEzVec2Maxi<float>);
    else IfTestExist(TestEzVec2Maxi<double>);
    else IfTestExist(TestEzVec2Maxi<int32_t>);
    else IfTestExist(TestEzVec2Maxi<int64_t>);
    else IfTestExist(TestEzVec2Maxi<uint32_t>);
    else IfTestExist(TestEzVec2Maxi<uint64_t>);

    IfTestExist(TestEzVec2Dot<float>);
    else IfTestExist(TestEzVec2Dot<double>);
    else IfTestExist(TestEzVec2Dot<int32_t>);
    else IfTestExist(TestEzVec2Dot<int64_t>);
    else IfTestExist(TestEzVec2Dot<uint32_t>);
    else IfTestExist(TestEzVec2Dot<uint64_t>);

    IfTestExist(TestEzVec2Det<float>);
    else IfTestExist(TestEzVec2Det<double>);
    else IfTestExist(TestEzVec2Det<int32_t>);
    else IfTestExist(TestEzVec2Det<int64_t>);
    else IfTestExist(TestEzVec2Det<uint32_t>);
    else IfTestExist(TestEzVec2Det<uint64_t>);

    IfTestExist(TestEzVec2Reflect<float>);
    else IfTestExist(TestEzVec2Reflect<double>);
    else IfTestExist(TestEzVec2Reflect<int32_t>);
    else IfTestExist(TestEzVec2Reflect<int64_t>);
    else IfTestExist(TestEzVec2Reflect<uint32_t>);
    else IfTestExist(TestEzVec2Reflect<uint64_t>);

    IfTestExist(TestEzVec2Sign<float>);
    else IfTestExist(TestEzVec2Sign<double>);
    else IfTestExist(TestEzVec2Sign<int32_t>);
    else IfTestExist(TestEzVec2Sign<int64_t>);
    else IfTestExist(TestEzVec2Sign<uint32_t>);
    else IfTestExist(TestEzVec2Sign<uint64_t>);

    IfTestExist(TestEzVec2Sin<float>);
    else IfTestExist(TestEzVec2Sin<double>);

    IfTestExist(TestEzVec2Cos<float>);
    else IfTestExist(TestEzVec2Cos<double>);

    IfTestExist(TestEzVec2Tan<float>);
    else IfTestExist(TestEzVec2Tan<double>);

    IfTestExist(TestEzVec2Atan<float>);
    else IfTestExist(TestEzVec2Atan<double>);

    return false;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
