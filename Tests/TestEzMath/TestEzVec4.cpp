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

template <typename T>
bool TestEzVec4Offset() {
    ez::vec4<T> v(1, 2, 3, 4);
    ez::vec4<T> offsetResult = v.Offset(1, 1, 1, 1);
    if (offsetResult.x != 2 || offsetResult.y != 3 || offsetResult.z != 4 || offsetResult.w != 5)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Set() {
    ez::vec4<T> v;
    v.Set(4, 5, 6, 7);
    if (v.x != 4 || v.y != 5 || v.z != 6 || v.w != 7)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Negate() {
    ez::vec4<T> v(1, -2, 3, -4);
    ez::vec4<T> negateResult = -v;
    if (negateResult.x != -1 || negateResult.y != 2 || negateResult.z != -3 || negateResult.w != 4)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4LogicalNot() {
    ez::vec4<T> v(1, 0, 3, 0);
    ez::vec4<T> notResult = !v;
    if (notResult.x != 0 || notResult.y != 1 || notResult.z != 0 || notResult.w != 1)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4XY() {
    ez::vec4<T> v(1, 2, 3, 4);
    ez::vec2<T> xyResult = v.xy();
    if (xyResult.x != 1 || xyResult.y != 2)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4XYZ() {
    ez::vec4<T> v(1, 2, 3, 4);
    ez::vec3<T> xyzResult = v.xyz();
    if (xyzResult.x != 1 || xyzResult.y != 2 || xyzResult.z != 3)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4ZW() {
    ez::vec4<T> v(1, 2, 3, 4);
    ez::vec2<T> zwResult = v.zw();
    if (zwResult.x != 3 || zwResult.y != 4)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Increment() {
    ez::vec4<T> v(1, 2, 3, 4);
    ++v;
    if (v.x != 2 || v.y != 3 || v.z != 4 || v.w != 5)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Decrement() {
    ez::vec4<T> v(1, 2, 3, 4);
    --v;
    if (v.x != 0 || v.y != 1 || v.z != 2 || v.w != 3)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Length() {
    ez::vec4<T> v(1, 2, 2, 1);
    T len = v.length();
    if (len != 3)
        return false;
    return true;
}

template <>
bool TestEzVec4Length<float>() {
    ez::vec4<float> v(1.0f, 2.0f, 2.0f, 1.0f);
    float len = v.length();
    if (std::abs(len - 3.0f) > 0.0001f)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Normalize() {
    ez::vec4<T> v(1, 2, 2, 1);
    v.normalize();
    if (std::abs(v.x - 1.0 / 3.0) > 0.0001)
        return false;
    if (std::abs(v.y - 2.0 / 3.0) > 0.0001)
        return false;
    if (std::abs(v.z - 2.0 / 3.0) > 0.0001)
        return false;
    if (std::abs(v.w - 1.0 / 3.0) > 0.0001)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Sum() {
    ez::vec4<T> v(1, 2, 3, 4);
    T sum = v.sum();
    if (sum != 10)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4SumAbs() {
    ez::vec4<T> v(1, -2, 3, -4);
    T sumAbs = v.sumAbs();
    if (sumAbs != 10)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4EmptyAND() {
    ez::vec4<T> v;
    if (!v.emptyAND())
        return false;
    v.x = 1;
    if (v.emptyAND())
        return false;
    return true;
}

template <typename T>
bool TestEzVec4EmptyOR() {
    ez::vec4<T> v(0, 1, 1, 1);
    if (!v.emptyOR())
        return false;
    v.x = 1;
    if (v.emptyOR())
        return false;
    return true;
}

template <typename T>
bool TestEzVec4String() {
    ez::vec4<T> v(1, 2, 3, 4);
    std::string str = v.string();
    if (str != "1;2;3;4")
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Mini() {
    ez::vec4<T> v(1, 2, 3, 4);
    T mini = v.mini();
    if (mini != 1)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Maxi() {
    ez::vec4<T> v(1, 2, 3, 4);
    T maxi = v.maxi();
    if (maxi != 4)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Equality() {
    ez::vec4<T> v1(1, 2, 3, 4);
    ez::vec4<T> v2(1, 2, 3, 4);
    if (!(v1 == v2))
        return false;
    ez::vec4<T> v3(5, 6, 7, 8);
    if (v1 == v3)
        return false;
    return true;
}

template <>
bool TestEzVec4Equality<float>() {
    ez::vec4<float> v1(1.0f, 2.0f, 3.0f, 4.0f);
    ez::vec4<float> v2(1.0f + 1e-6f, 2.0f - 1e-6f, 3.0f + 1e-6f, 4.0f - 1e-6f);
    if (!(v1 == v2))  // Assuming isEqual is used in operator==
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Addition() {
    ez::vec4<T> v1(1, 2, 3, 4);
    ez::vec4<T> v2(5, 6, 7, 8);
    ez::vec4<T> result = v1 + v2;
    if (result.x != 6 || result.y != 8 || result.z != 10 || result.w != 12)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Subtraction() {
    ez::vec4<T> v1(5, 6, 7, 8);
    ez::vec4<T> v2(1, 2, 3, 4);
    ez::vec4<T> result = v1 - v2;
    if (result.x != 4 || result.y != 4 || result.z != 4 || result.w != 4)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Multiplication() {
    ez::vec4<T> v(1, 2, 3, 4);
    ez::vec4<T> result = v * static_cast<T>(2);
    if (result.x != 2 || result.y != 4 || result.z != 6 || result.w != 8)
        return false;
    return true;
}

template <typename T>
bool TestEzVec4Division() {
    ez::vec4<T> v(4, 8, 12, 16);
    ez::vec4<T> result = v / static_cast<T>(2);
    if (result.x != 2 || result.y != 4 || result.z != 6 || result.w != 8)
        return false;
    return true;
}

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

bool TestEzVec4(const std::string& vTest) {
    IfTestExist(TestEzVec4Offset<float>);
    else IfTestExist(TestEzVec4Offset<double>);
    else IfTestExist(TestEzVec4Offset<int32_t>);
    else IfTestExist(TestEzVec4Offset<int64_t>);
    else IfTestExist(TestEzVec4Offset<uint32_t>);
    else IfTestExist(TestEzVec4Offset<uint64_t>);

    IfTestExist(TestEzVec4Set<float>);
    else IfTestExist(TestEzVec4Set<double>);
    else IfTestExist(TestEzVec4Set<int32_t>);
    else IfTestExist(TestEzVec4Set<int64_t>);

    IfTestExist(TestEzVec4Negate<float>);
    else IfTestExist(TestEzVec4Negate<double>);
    else IfTestExist(TestEzVec4Negate<int32_t>);
    else IfTestExist(TestEzVec4Negate<int64_t>);

    IfTestExist(TestEzVec4LogicalNot<int32_t>);
    else IfTestExist(TestEzVec4LogicalNot<int64_t>);

    IfTestExist(TestEzVec4XY<float>);
    else IfTestExist(TestEzVec4XY<double>);
    else IfTestExist(TestEzVec4XY<int32_t>);
    else IfTestExist(TestEzVec4XY<int64_t>);

    IfTestExist(TestEzVec4XYZ<float>);
    else IfTestExist(TestEzVec4XYZ<double>);
    else IfTestExist(TestEzVec4XYZ<int32_t>);
    else IfTestExist(TestEzVec4XYZ<int64_t>);

    IfTestExist(TestEzVec4ZW<float>);
    else IfTestExist(TestEzVec4ZW<double>);
    else IfTestExist(TestEzVec4ZW<int32_t>);
    else IfTestExist(TestEzVec4ZW<int64_t>);

    IfTestExist(TestEzVec4Increment<float>);
    else IfTestExist(TestEzVec4Increment<double>);
    else IfTestExist(TestEzVec4Increment<int32_t>);
    else IfTestExist(TestEzVec4Increment<int64_t>);

    IfTestExist(TestEzVec4Decrement<float>);
    else IfTestExist(TestEzVec4Decrement<double>);
    else IfTestExist(TestEzVec4Decrement<int32_t>);
    else IfTestExist(TestEzVec4Decrement<int64_t>);

    IfTestExist(TestEzVec4Length<float>);
    else IfTestExist(TestEzVec4Length<double>);
    else IfTestExist(TestEzVec4Length<int32_t>);
    else IfTestExist(TestEzVec4Length<int64_t>);

    IfTestExist(TestEzVec4Normalize<float>);
    else IfTestExist(TestEzVec4Normalize<double>);
    else IfTestExist(TestEzVec4Normalize<int32_t>);
    else IfTestExist(TestEzVec4Normalize<int64_t>);

    IfTestExist(TestEzVec4Sum<float>);
    else IfTestExist(TestEzVec4Sum<double>);
    else IfTestExist(TestEzVec4Sum<int32_t>);
    else IfTestExist(TestEzVec4Sum<int64_t>);

    IfTestExist(TestEzVec4SumAbs<float>);
    else IfTestExist(TestEzVec4SumAbs<double>);
    else IfTestExist(TestEzVec4SumAbs<int32_t>);
    else IfTestExist(TestEzVec4SumAbs<int64_t>);

    IfTestExist(TestEzVec4EmptyAND<float>);
    else IfTestExist(TestEzVec4EmptyAND<double>);
    else IfTestExist(TestEzVec4EmptyAND<int32_t>);
    else IfTestExist(TestEzVec4EmptyAND<int64_t>);

    IfTestExist(TestEzVec4EmptyOR<float>);
    else IfTestExist(TestEzVec4EmptyOR<double>);
    else IfTestExist(TestEzVec4EmptyOR<int32_t>);
    else IfTestExist(TestEzVec4EmptyOR<int64_t>);

    IfTestExist(TestEzVec4String<float>);
    else IfTestExist(TestEzVec4String<double>);
    else IfTestExist(TestEzVec4String<int32_t>);
    else IfTestExist(TestEzVec4String<int64_t>);

    IfTestExist(TestEzVec4Mini<float>);
    else IfTestExist(TestEzVec4Mini<double>);
    else IfTestExist(TestEzVec4Mini<int32_t>);
    else IfTestExist(TestEzVec4Mini<int64_t>);

    IfTestExist(TestEzVec4Maxi<float>);
    else IfTestExist(TestEzVec4Maxi<double>);
    else IfTestExist(TestEzVec4Maxi<int32_t>);
    else IfTestExist(TestEzVec4Maxi<int64_t>);

    IfTestExist(TestEzVec4Equality<float>);
    else IfTestExist(TestEzVec4Equality<double>);
    else IfTestExist(TestEzVec4Equality<int32_t>);
    else IfTestExist(TestEzVec4Equality<int64_t>);

    IfTestExist(TestEzVec4Addition<float>);
    else IfTestExist(TestEzVec4Addition<double>);
    else IfTestExist(TestEzVec4Addition<int32_t>);
    else IfTestExist(TestEzVec4Addition<int64_t>);

    IfTestExist(TestEzVec4Subtraction<float>);
    else IfTestExist(TestEzVec4Subtraction<double>);
    else IfTestExist(TestEzVec4Subtraction<int32_t>);
    else IfTestExist(TestEzVec4Subtraction<int64_t>);

    IfTestExist(TestEzVec4Multiplication<float>);
    else IfTestExist(TestEzVec4Multiplication<double>);
    else IfTestExist(TestEzVec4Multiplication<int32_t>);
    else IfTestExist(TestEzVec4Multiplication<int64_t>);

    IfTestExist(TestEzVec4Division<float>);
    else IfTestExist(TestEzVec4Division<double>);
    else IfTestExist(TestEzVec4Division<int32_t>);
    else IfTestExist(TestEzVec4Division<int64_t>);

    return false;  // Return false if the test case is not found
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
