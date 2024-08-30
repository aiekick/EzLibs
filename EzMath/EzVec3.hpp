#pragma once

namespace ez {
template <typename T>
struct vec3 {
    T x = (T)0, y = (T)0, z = (T)0;
    vec3() : x((T)0), y((T)0), z((T)0) {
    }
    template <typename U>
    vec3<T>(vec3<U> a) {
        x = (T)a.x;
        y = (T)a.y;
        z = (T)a.z;
    }
    vec3(const T& xyz) : x(xyz), y(xyz), z(xyz) {
    }
    vec3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {
    }
    vec3(const vec2<T>& xy, const T& z) : x(xy.x), y(xy.y), z(z) {
    }
    vec3(const std::string& vec, const char& c = ';', vec3<T>* def = nullptr)  // may be in format "0.2f,0.3f,0.4f"
    {
        if (def) {
            x = def->x;
            y = def->y;
            z = def->z;
        }
        std::vector<T> result = StringToNumberVector<T>(vec, c);
        const size_t s = result.size();
        if (s > 0)
            x = result[0];
        if (s > 1)
            y = result[1];
        if (s > 2)
            z = result[2];
    }
    T& operator[](const size_t& i) {
        return (&x)[i];
    }
    vec3<T> Offset(const T& vX, const T& vY, const T& vZ) const {
        return vec3<T>(x + vX, y + vY, z + vZ);
    }
    void Set(const T& vX, const T& vY, const T& vZ) {
        x = vX;
        y = vY;
        z = vZ;
    }
    vec3<T> operator-() const {
        return vec3<T>(-x, -y, -z);
    }
    vec3<T> operator!() const {
        return vec3<T>(!(bool)x, !(bool)y, !(bool)z);
    }
    vec2<T> xy() const {
        return vec2<T>(x, y);
    }
    vec2<T> xz() const {
        return vec2<T>(x, z);
    }
    vec2<T> yz() const {
        return vec2<T>(y, z);
    }
    vec3 yzx() const {
        return vec3<T>(y, z, x);
    }
    // https://en.cppreference.com/w/cpp/language/operator_incdec
    vec3<T>& operator++() {
        ++x;
        ++y;
        ++z;
        return *this;
    }  // pre inc
    vec3<T>& operator--() {
        --x;
        --y;
        --z;
        return *this;
    }  // pre dec
    vec3<T> operator++(int) {
        vec3<T> tmp = *this;
        ++*this;
        return tmp;
    }  // post inc
    vec3<T> operator--(int) {
        vec3<T> tmp = *this;
        --*this;
        return tmp;
    }  // post dec
    void operator+=(const T& a) {
        x += a;
        y += a;
        z += a;
    }
    void operator+=(const vec3<T>& v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    void operator-=(const T& a) {
        x -= a;
        y -= a;
        z -= a;
    }
    void operator-=(const vec3<T>& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    bool operator==(const T& a) {
        return (x == a) && (y == a) && (z == a);
    }
    bool operator==(const vec3<T>& v) {
        return (x == v.x) && (y == v.y) && (z == v.z);
    }
    bool operator!=(const T& a) {
        return (x != a) || (y != a) || (z != a);
    }
    bool operator!=(const vec3<T>& v) {
        return (x != v.x) || (y != v.y) || (z != v.z);
    }
    void operator*=(const T& a) {
        x *= a;
        y *= a;
        z *= a;
    }
    void operator*=(const vec3<T>& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }
    void operator/=(const T& a) {
        x /= a;
        y /= a;
        z /= a;
    }
    void operator/=(const vec3<T>& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
    }
    T length() const {
        return (T)sqrt(lengthSquared());
    }
    T lengthSquared() const {
        return x * x + y * y + z * z;
    }
    T normalize() {
        T _length = length();
        if (_length < std::numeric_limits<T>::epsilon())
            return (T)0;
        T _invLength = (T)1 / _length;
        x *= _invLength;
        y *= _invLength;
        z *= _invLength;
        return _length;
    }
    vec3<T> GetNormalized() const {
        vec3<T> n = vec3<T>(x, y, z);
        n.normalize();
        return n;
    }
    T sum() const {
        return x + y + z;
    }
    T sumAbs() const {
        return abs<T>(x) + abs<T>(y) + abs<T>(z);
    }
    bool emptyAND() const {
        return x == (T)0 && y == (T)0 && z == (T)0;
    }
    bool emptyOR() const {
        return x == (T)0 || y == (T)0 || z == (T)0;
    }
    std::string string(const char& c = ';') const {
        return toStr(x) + c + toStr(y) + c + toStr(z);
    }
    T mini() const {
        return internal_mini<T>(x, internal_mini<T>(y, z));
    }
    T maxi() const {
        return internal_maxi<T>(x, internal_maxi<T>(y, z));
    }
};
// https://en.cppreference.com/w/cpp/language/operator_incdec
template <typename T>
inline vec3<T>& operator++(vec3<T>& v) {
    ++v;
    return v;
}  // pre inc
template <typename T>
inline vec3<T>& operator--(vec3<T>& v) {
    --v;
    return v;
}  // pre dec
template <typename T>
inline vec3<T> operator++(vec3<T>& v, int) {
    vec3<T> a = v;
    ++a;
    return a;
}  // post inc
template <typename T>
inline vec3<T> operator--(vec3<T>& v, int) {
    vec3<T> a = v;
    --a;
    return a;
}  // post dec
template <typename T>
inline vec3<T> operator+(vec3<T> v, T f) {
    return vec3<T>(v.x + f, v.y + f, v.z + f);
}
template <typename T>
inline vec3<T> operator+(vec3<T> v, vec3<T> f) {
    return vec3<T>(v.x + f.x, v.y + f.y, v.z + f.z);
}
template <typename T>
inline vec3<T> operator+(T f, vec3<T> v) {
    return vec3<T>(f + v.x, f + v.y, f + v.z);
}
template <typename T>
inline vec3<T> operator-(vec3<T> v, T f) {
    return vec3<T>(v.x - f, v.y - f, v.z - f);
}
template <typename T>
inline vec3<T> operator-(vec3<T> v, vec3<T> f) {
    return vec3<T>(v.x - f.x, v.y - f.y, v.z - f.z);
}
template <typename T>
inline vec3<T> operator-(T f, vec3<T> v) {
    return vec3<T>(f - v.x, f - v.y, f - v.z);
}
template <typename T>
inline vec3<T> operator*(vec3<T> v, T f) {
    return vec3<T>(v.x * f, v.y * f, v.z * f);
}
template <typename T>
inline vec3<T> operator*(vec3<T> v, vec3<T> f) {
    return vec3<T>(v.x * f.x, v.y * f.y, v.z * f.z);
}
template <typename T>
inline vec3<T> operator*(T f, vec3<T> v) {
    return vec3<T>(f * v.x, f * v.y, f * v.z);
}
template <typename T>
inline vec3<T> operator/(vec3<T> v, T f) {
    return vec3<T>(v.x / f, v.y / f, v.z / f);
}
template <typename T>
inline vec3<T> operator/(T f, vec3<T> v) {
    return vec3<T>(f / v.x, f / v.y, f / v.z);
}
template <typename T>
inline vec3<T> operator/(vec3<T> v, vec3<T> f) {
    return vec3<T>(v.x / f.x, v.y / f.y, v.z / f.z);
}
template <typename T>
inline bool operator<(vec3<T> v, vec3<T> f) {
    return v.x < f.x && v.y < f.y && v.z < f.z;
}
template <typename T>
inline bool operator<(vec3<T> v, T f) {
    return v.x < f && v.y < f && v.z < f;
}
template <typename T>
inline bool operator>(vec3<T> v, vec3<T> f) {
    return v.x > f.x && v.y > f.y && v.z > f.z;
}
template <typename T>
inline bool operator>(vec3<T> v, T f) {
    return v.x > f && v.y > f && v.z > f;
}
template <typename T>
inline bool operator<=(vec3<T> v, vec3<T> f) {
    return v.x <= f.x && v.y <= f.y && v.z <= f.z;
}
template <typename T>
inline bool operator<=(vec3<T> v, T f) {
    return v.x <= f && v.y <= f && v.z <= f;
}
template <typename T>
inline bool operator>=(vec3<T> v, vec3<T> f) {
    return v.x >= f.x && v.y >= f.y && v.z >= f.z;
}
template <typename T>
inline bool operator>=(vec3<T> v, T f) {
    return v.x >= f && v.y >= f && v.z >= f;
}
template <typename T>
inline bool operator!=(vec3<T> v, vec3<T> f) {
    return f.x != v.x || f.y != v.y || f.z != v.z;
}
template <typename T>
inline bool operator==(vec3<T> v, vec3<T> f) {
    return f.x == v.x && f.y == v.y && f.z == v.z;
}
template <typename T>
inline vec3<T> mini(vec3<T> a, vec3<T> b) {
    return vec3<T>(mini<T>(a.x, b.x), mini<T>(a.y, b.y), mini<T>(a.z, b.z));
}
template <typename T>
inline vec3<T> maxi(vec3<T> a, vec3<T> b) {
    return vec3<T>(maxi<T>(a.x, b.x), maxi<T>(a.y, b.y), maxi<T>(a.z, b.z));
}
template <typename T>
inline vec3<T> floor(vec3<T> a) {
    return vec3<T>(floor<T>(a.x), floor<T>(a.y), floor<T>(a.z));
}
template <typename T>
inline vec3<T> ceil(vec3<T> a) {
    return vec3<T>(ceil<T>(a.x), ceil<T>(a.y), ceil<T>(a.z));
}
template <typename T>
inline vec2<T> abs(vec2<T> a) {
    return vec2<T>(abs<T>(a.x), abs<T>(a.y));
}
template <typename T>
inline vec3<T> abs(vec3<T> a) {
    return vec3<T>(abs<T>(a.x), abs<T>(a.y), abs<T>(a.z));
}
template <typename T>
inline T dotS(vec3<T> a, vec3<T> b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
template <typename T>
inline vec3<T> cCross(vec3<T> a, vec3<T> b) {
    return vec3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
template <typename T>
inline vec3<T> cReflect(vec3<T> I, vec3<T> N) {
    return I - (T)2 * dotS(N, I) * N;
}
using dvec3 = vec3<double>;
using fvec3 = vec3<float>;
using bvec3 = vec3<bool>;
using ivec3 = vec3<int>;
using i8vec3 = vec3<int8_t>;
using i16vec3 = vec3<int16_t>;
using ivec3 = vec3<int32_t>;
using i32vec3 = vec3<int32_t>;
using i64vec3 = vec3<int64_t>;
using u8vec3 = vec3<uint8_t>;
using u16vec3 = vec3<uint16_t>;
using uvec3 = vec3<uint32_t>;
using u32vec3 = vec3<uint32_t>;
using u64vec3 = vec3<uint64_t>;

// specialization for float32 test to fvec3
inline bool valid(const fvec3& a) {
    return floatIsValid(a.x) && floatIsValid(a.y) && floatIsValid(a.z);
}
// specialization for fvec2
inline bool operator==(const fvec3& v, const fvec3& f) {
    return isFloatEqual(f.x, v.x) && isFloatEqual(f.y, v.y) && isFloatEqual(f.z, v.z);
}
inline bool operator!=(const fvec3& v, const fvec3& f) {
    return isFloatDifferent(f.x, v.x) || isFloatDifferent(f.y, v.y) || isFloatDifferent(f.z, v.z);
}

}  // namespace ez
