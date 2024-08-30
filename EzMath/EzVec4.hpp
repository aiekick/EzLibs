#pragma once

namespace ez {

template <typename T>
struct vec4 {
    T x = (T)0, y = (T)0, z = (T)0, w = (T)0;
    vec4() : x((T)0), y((T)0), z((T)0), w((T)0) {
    }
    template <typename U>
    vec4<T>(vec4<U> a) {
        x = (T)a.x;
        y = (T)a.y;
        z = (T)a.z;
        w = (T)a.w;
    }
    vec4(vec2<T> xy, vec2<T> zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {
    }
    vec4(vec3<T> xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {
    }
    vec4(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {
    }
    vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {
    }
#ifdef USE_IMGUI
    vec4<T>(const ImVec4& vec) {
        x = (T)vec.x;
        y = (T)vec.y;
        z = (T)vec.z;
        w = (T)vec.w;
    }
#endif
    vec4(std::string vec, char c = ';', vec4<T>* def = nullptr)  // may be in format "0.2f,0.3f,0.4f,0.8f"
    {
        if (def) {
            x = def->x;
            y = def->y;
            z = def->z;
            w = def->w;
        }
        std::vector<T> result = StringToNumberVector<T>(vec, c);
        const size_t s = result.size();
        if (s > 0)
            x = result[0];
        if (s > 1)
            y = result[1];
        if (s > 2)
            z = result[2];
        if (s > 3)
            w = result[3];
    }
    vec4(std::string vec, char c = ';', int n = 4, vec4<T>* def = nullptr)  // may be in format "0.2f,0.3f,0.4f,0.8f"
    {
        if (def) {
            x = def->x;
            y = def->y;
            z = def->z;
            w = def->w;
        } else {
            x = (T)0;
            y = (T)0;
            z = (T)0;
            w = (T)0;
        }
        std::vector<T> result = StringToNumberVector<T>(vec, c);
        const size_t s = result.size();
        if ((int)s != n) {
            if (s == 1) {
                x = result[0];
                y = result[0];
                z = result[0];
                w = result[0];
            } else if (s == 2) {
                x = result[0];
                y = result[0];
                z = result[1];
                w = result[1];
            } else if (s == 3) {
                x = result[0];
                y = result[1];
                z = result[2];
            }
        } else {
            if (s > 0)
                x = result[0];
            if (s > 1)
                y = result[1];
            if (s > 2)
                z = result[2];
            if (s > 3)
                w = result[3];
        }
    }
    T& operator[](const size_t& i) {
        return (&x)[i];
    }
    vec4<T> Offset(const T& vX, const T& vY, const T& vZ, const T& vW) const {
        return vec4<T>(x + vX, y + vY, z + vZ, w + vW);
    }
    void Set(const T& vX, const T& vY, const T& vZ, const T& vW) {
        x = vX;
        y = vY;
        z = vZ;
        w = vW;
    }
    vec4<T> operator-() const {
        return vec4<T>(-x, -y, -z, -w);
    }
    vec4<T> operator!() const {
        return vec4<T>(!(bool)x, !(bool)y, !(bool)z, !(bool)w);
    }
    vec2<T> xy() const {
        return vec2<T>(x, y);
    }
    vec3<T> xyz() const {
        return vec3<T>(x, y, z);
    }
    vec2<T> zw() const {
        return vec2<T>(z, w);
    }
    // https://en.cppreference.com/w/cpp/language/operator_incdec
    vec4<T>& operator++() {
        ++x;
        ++y;
        ++z;
        ++w;
        return *this;
    }  // pre inc
    vec4<T>& operator--() {
        --x;
        --y;
        --z;
        --w;
        return *this;
    }  // pre dec
    vec4<T> operator++(int) {
        vec4<T> tmp = *this;
        ++*this;
        return tmp;
    }  // post inc
    vec4<T> operator--(int) {
        vec4<T> tmp = *this;
        --*this;
        return tmp;
    }  // post dec
    void operator+=(const T& a) {
        x += a;
        y += a;
        z += a;
        w += a;
    }
    void operator+=(const vec4<T>& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
    }
    void operator-=(const T& a) {
        x -= a;
        y -= a;
        z -= a;
        w -= a;
    }
    void operator-=(const vec4<T>& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }
    bool operator==(const T& a) {
        return (x == a) && (y == a) && (z == a) && (w == a);
    }
    bool operator==(const vec4<T>& v) {
        return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
    }
    bool operator!=(const T& a) {
        return (x != a) || (y != a) || (z != a) || (w != a);
    }
    bool operator!=(const vec4<T>& v) {
        return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
    }
    void operator*=(const T& a) {
        x *= a;
        y *= a;
        z *= a;
        w *= a;
    }
    void operator*=(const vec4<T>& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
    }
    void operator/=(const T& a) {
        x /= a;
        y /= a;
        z /= a;
        w /= a;
    }
    void operator/=(const vec4<T>& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
    }
    vec2<T> SizeLBRT() const {
        return vec2<T>(z - x, w - y);
    }  // Considere vec4 as a rect with LBRT for have size LBRT Mean => x = Left, y = Bottom, z = Right, w = Top
    vec2<T> pos() const {
        return xy();
    }
    vec2<T> size() const {
        return zw();
    }
    T length() const {
        return (T)sqrt(lengthSquared());
    }
    T lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    T normalize() {
        T _length = length();
        if (_length < (T)1e-5)
            return (T)0;
        T _invLength = (T)1 / _length;
        x *= _invLength;
        y *= _invLength;
        z *= _invLength;
        w *= _invLength;
        return _length;
    }
    vec4<T> GetNormalized() const {
        vec4<T> n = vec4<T>(x, y, z, w);
        n.normalize();
        return n;
    }
    bool emptyAND() const {
        return x == (T)0 && y == (T)0 && z == (T)0 && w == (T)0;
    }
    bool emptyOR() const {
        return x == (T)0 || y == (T)0 || z == (T)0 || w == (T)0;
    }
    T sum() const {
        return x + y + z + w;
    }
    T sumAbs() const {
        return abs<T>(x) + abs<T>(y) + abs<T>(z) + abs<T>(w);
    }
    std::string string(char c = ';') const {
        return toStr(x) + c + toStr(y) + c + toStr(z) + c + toStr(w);
    }
    T mini() const {
        return internal_mini<T>(x, internal_mini<T>(y, internal_mini<T>(z, w)));
    }
    T maxi() const {
        return internal_maxi<T>(x, internal_maxi<T>(y, internal_maxi<T>(z, w)));
    }
};

// https://en.cppreference.com/w/cpp/language/operator_incdec
template <typename T>
inline vec4<T>& operator++(vec4<T>& v) {
    ++v;
    return v;
}  // pre inc
template <typename T>
inline vec4<T>& operator--(vec4<T>& v) {
    --v;
    return v;
}  // pre dec
template <typename T>
inline vec4<T> operator++(vec4<T>& v, int) {
    vec4<T> a = v;
    ++a;
    return a;
}  // post inc
template <typename T>
inline vec4<T> operator--(vec4<T>& v, int) {
    vec4<T> a = v;
    --a;
    return a;
}  // post dec
template <typename T>
inline vec4<T> operator+(vec4<T> v, T f) {
    return vec4<T>(v.x + f, v.y + f, v.z + f, v.w + f);
}
template <typename T>
inline vec4<T> operator+(vec4<T> v, vec4<T> f) {
    return vec4<T>(v.x + f.x, v.y + f.y, v.z + f.z, v.w + f.w);
}
template <typename T>
inline vec4<T> operator-(vec4<T> v, T f) {
    return vec4<T>(v.x - f, v.y - f, v.z - f, v.w - f);
}
template <typename T>
inline vec4<T> operator-(T f, vec4<T> v) {
    return vec4<T>(f - v.x, f - v.y, f - v.z, f - v.w);
}
template <typename T>
inline vec4<T> operator-(vec4<T> v, vec4<T> f) {
    return vec4<T>(v.x - f.x, v.y - f.y, v.z - f.z, v.w - f.w);
}
template <typename T>
inline vec4<T> operator*(vec4<T> v, T f) {
    return vec4<T>(v.x * f, v.y * f, v.z * f, v.w * f);
}
template <typename T>
inline vec4<T> operator*(vec4<T> v, vec4<T> f) {
    return vec4<T>(v.x * f.x, v.y * f.y, v.z * f.z, v.w * f.w);
}
template <typename T>
inline vec4<T> operator/(vec4<T> v, T f) {
    return vec4<T>(v.x / f, v.y / f, v.z / f, v.w / f);
}
template <typename T>
inline vec4<T> operator/(vec4<T> v, vec4<T> f) {
    return vec4<T>(v.x / f.x, v.y / f.y, v.z / f.z, v.w / f.w);
}
template <typename T>
inline bool operator<(vec4<T> v, vec4<T> f) {
    return v.x < f.x && v.y < f.y && v.z < f.z && v.w < f.w;
}
template <typename T>
inline bool operator<(vec4<T> v, T f) {
    return v.x < f && v.y < f && v.z < f && v.w < f;
}
template <typename T>
inline bool operator>(vec4<T> v, vec4<T> f) {
    return v.x > f.x && v.y > f.y && v.z > f.z && v.w > f.w;
}
template <typename T>
inline bool operator>(vec4<T> v, T f) {
    return v.x > f && v.y > f && v.z > f && v.w > f;
}
template <typename T>
inline bool operator<=(vec4<T> v, vec4<T> f) {
    return v.x <= f.x && v.y <= f.y && v.z <= f.z && v.w <= f.w;
}
template <typename T>
inline bool operator<=(vec4<T> v, T f) {
    return v.x <= f && v.y <= f && v.z <= f && v.w <= f;
}
template <typename T>
inline bool operator>=(vec4<T> v, vec4<T> f) {
    return v.x >= f.x && v.y >= f.y && v.z >= f.z && v.w >= f.w;
}
template <typename T>
inline bool operator>=(vec4<T> v, T f) {
    return v.x >= f && v.y >= f && v.z >= f && v.w >= f;
}
template <typename T>
inline bool operator==(vec4<T> v, vec4<T> f) {
    return f.x == v.x && f.y == v.y && f.z == v.z && f.w == v.w;
}
template <typename T>
inline bool operator!=(vec4<T> v, vec4<T> f) {
    return f.x != v.x || f.y != v.y || f.z != v.z || f.w != v.w;
}
template <typename T>
inline vec4<T> mini(vec4<T> a, vec4<T> b) {
    return vec4<T>(mini<T>(a.x, b.x), mini<T>(a.y, b.y), mini<T>(a.z, b.z), mini<T>(a.w, b.w));
}
template <typename T>
inline vec4<T> maxi(vec4<T> a, vec4<T> b) {
    return vec4<T>(maxi<T>(a.x, b.x), maxi<T>(a.y, b.y), maxi<T>(a.z, b.z), maxi<T>(a.w, b.w));
}
template <typename T>
inline vec4<T> floor(vec4<T> a) {
    return vec4<T>(floor<T>(a.x), floor<T>(a.y), floor<T>(a.z), floor<T>(a.w));
}
template <typename T>
inline vec4<T> ceil(vec4<T> a) {
    return vec4<T>(ceil<T>(a.x), ceil<T>(a.y), ceil<T>(a.z), ceil<T>(a.w));
}
template <typename T>
inline vec4<T> abs(vec4<T> a) {
    return vec4<T>(abs<T>(a.x), abs<T>(a.y), abs<T>(a.z), abs<T>(a.w));
}
template <typename T>
inline vec4<T> sign(vec4<T> a) {
    return vec4<T>(a.x < (T)0 ? (T)-1 : (T)1, a.y < (T)0 ? (T)-1 : 1, a.z < (T)0 ? (T)-1 : (T)1, a.w < (T)0 ? (T)-1 : (T)1);
}
template <typename T>
inline vec4<T> sin(vec4<T> a) {
    return vec4<T>(sin<T>(a.x), sin<T>(a.y), sin<T>(a.z), sin<T>(a.w));
}
template <typename T>
inline vec4<T> cos(vec4<T> a) {
    return vec4<T>(cos<T>(a.x), cos<T>(a.y), cos<T>(a.z), cos<T>(a.w));
}
template <typename T>
inline vec4<T> tan(vec4<T> a) {
    return vec4<T>(tan<T>(a.x), tan<T>(a.y), tan<T>(a.z), tan<T>(a.w));
}
using dvec4 = vec4<double>;
using fvec4 = vec4<float>;
using bvec4 = vec4<bool>;
using ivec4 = vec4<int>;
using i8vec4 = vec4<int8_t>;
using i16vec4 = vec4<int16_t>;
using ivec4 = vec4<int32_t>;
using i32vec4 = vec4<int32_t>;
using i64vec4 = vec4<int64_t>;
using u8vec4 = vec4<uint8_t>;
using u16vec4 = vec4<uint16_t>;
using uvec4 = vec4<uint32_t>;
using u32vec4 = vec4<uint32_t>;
using u64vec4 = vec4<uint64_t>;

// specialization for float32 test to fvec4
inline bool valid(const fvec4& a) {
    return floatIsValid(a.x) && floatIsValid(a.y) && floatIsValid(a.z) && floatIsValid(a.w);
}

// specialization for fvec4
inline bool operator==(const fvec4& v, const fvec4& f) {
    return isFloatEqual(f.x, v.x) && isFloatEqual(f.y, v.y) && isFloatEqual(f.z, v.z) && isFloatEqual(f.w, v.w);
}
inline bool operator!=(const fvec4& v, const fvec4& f) {
    return isFloatDifferent(f.x, v.x) || isFloatDifferent(f.y, v.y) || isFloatDifferent(f.z, v.z) || isFloatDifferent(f.w, v.w);
}

}  // namespace ez
