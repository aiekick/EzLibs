#pragma once

namespace ez {
template <typename T>
struct vec2 {
    T x = (T)0, y = (T)0;
    vec2<T>() {
        x = (T)0;
        y = (T)0;
    }
    template <typename U>
    vec2<T>(const vec2<U>& a) {
        x = (T)a.x;
        y = (T)a.y;
    }
    vec2<T>(const T& a) {
        x = a;
        y = a;
    }
    vec2<T>(const T& a, const T& b) {
        x = a;
        y = b;
    }
    vec2<T>(const std::string& vec, const char& c = ';', vec2<T>* def = nullptr)  // may be in format "0.2f,0.3f,0.4f"
    {
        if (def) {
            x = def->x;
            y = def->y;
        }
        std::vector<T> result = StringToNumberVector<T>(vec, c);
        const size_t s = result.size();
        if (s > 0)
            x = result[0];
        if (s > 1)
            y = result[1];
    }
#ifdef USE_IMGUI
    vec2<T>(const ImVec2& vec) {
        x = (T)vec.x;
        y = (T)vec.y;
    }
#endif
#ifdef USE_BOX2D
    vec2<T>(const b2Vec2& vec, const float& vRatio = 1.0f) {
        x = (T)vec.x * vRatio;
        y = (T)vec.y * vRatio;
    }
#endif
    T& operator[](const size_t& i) {
        return (&x)[i];
    }
    vec2<T> Offset(const T& vX, const T& vY) const {
        return vec2<T>(x + vX, y + vY);
    }
    void Set(const T& vX, const T& vY) {
        x = vX;
        y = vY;
    }
    vec2<T> operator-() const {
        return vec2<T>(-x, -y);
    }
    vec2<T> operator!() const {
        return vec2<T>(!(bool)x, !(bool)y);
    }
    vec2<T>& operator++() {
        ++x;
        ++y;
        return *this;
    }  // pre inc
    vec2<T>& operator--() {
        --x;
        --y;
        return *this;
    }  // pre dec
    vec2<T> operator++(int) {
        vec2<T> tmp = *this;
        ++*this;
        return tmp;
    }  // post inc
    vec2<T> operator--(int) {
        vec2<T> tmp = *this;
        --*this;
        return tmp;
    }  // post dec
    void operator+=(const T& a) {
        x += a;
        y += a;
    }
    void operator+=(const vec2<T>& v) {
        x += v.x;
        y += v.y;
    }
    void operator-=(const T& a) {
        x -= a;
        y -= a;
    }
    void operator-=(const vec2<T>& v) {
        x -= v.x;
        y -= v.y;
    }
    bool operator==(const T& a) {
        return (x == a) && (y == a);
    }
    bool operator==(const vec2<T>& v) {
        return (x == v.x) && (y == v.y);
    }
    bool operator!=(const T& a) {
        return (x != a) || (y != a);
    }
    bool operator!=(const vec2<T>& v) {
        return (x != v.x) || (y != v.y);
    }
    void operator*=(const T& a) {
        x *= a;
        y *= a;
    }
    void operator*=(const vec2<T>& v) {
        x *= v.x;
        y *= v.y;
    }
    void operator/=(const T& a) {
        x /= a;
        y /= a;
    }
    void operator/=(const vec2<T>& v) {
        x /= v.x;
        y /= v.y;
    }
    T lengthSquared() const {
        return x * x + y * y;
    }
    T length() const {
        return (T)sqrt(lengthSquared());
    }
    T normalize() {
        T _length = length();
        if (_length < (T)1e-5)
            return (T)0.0;
        T _invLength = (T)1.0 / _length;
        x *= _invLength;
        y *= _invLength;
        return _length;
    }  // return length
    vec2<T> GetNormalized() const {
        vec2<T> n = vec2<T>(x, y);
        n.normalize();
        return n;
    }
    T sum() const {
        return x + y;
    }
    T sumAbs() const {
        return abs<T>(x) + abs<T>(y);
    }
    bool emptyAND() const {
        return x == (T)0 && y == (T)0;
    }
    bool emptyOR() const {
        return x == (T)0 || y == (T)0;
    }
    std::string string(const char& c = ';') const {
        return toStr(x) + c + toStr(y);
    }
    template <typename U>
    U ratioXY() const {
        if (y != (T)0)
            return (U)x / (U)y;
        return (U)0;
    }
    template <typename U>
    U ratioYX() const {
        if (x != (T)0)
            return (U)y / (U)x;
        return (U)0;
    }
    T mini() const {
        return internal_mini<T>(x, y);
    }
    T maxi() const {
        return internal_maxi<T>(x, y);
    }
};

// https://en.cppreference.com/w/cpp/language/operator_incdec
template <typename T>
inline vec2<T>& operator++(vec2<T>& v) {
    ++v;
    return v;
}  // pre inc
template <typename T>
inline vec2<T>& operator--(vec2<T>& v) {
    --v;
    return v;
}  // pre dec
template <typename T>
inline vec2<T> operator++(vec2<T>& v, int) {
    vec2<T> a = v;
    ++a;
    return a;
}  // post inc
template <typename T>
inline vec2<T> operator--(vec2<T>& v, int) {
    vec2<T> a = v;
    --a;
    return a;
}  // post dec
template <typename T>
inline vec2<T> operator+(vec2<T> v, T f) {
    return vec2<T>(v.x + f, v.y + f);
}
template <typename T>
inline vec2<T> operator+(T f, vec2<T> v) {
    return vec2<T>(v.x + f, v.y + f);
}
template <typename T>
inline vec2<T> operator+(vec2<T> v, vec2<T> f) {
    return vec2<T>(v.x + f.x, v.y + f.y);
}
template <typename T>
inline vec2<T> operator-(vec2<T> v, T f) {
    return vec2<T>(v.x - f, v.y - f);
}
template <typename T>
inline vec2<T> operator-(T f, vec2<T> v) {
    return vec2<T>(f - v.x, f - v.y);
}
template <typename T>
inline vec2<T> operator-(vec2<T> v, vec2<T> f) {
    return vec2<T>(v.x - f.x, v.y - f.y);
}
template <typename T>
inline vec2<T> operator*(vec2<T> v, T f) {
    return vec2<T>(v.x * f, v.y * f);
}
template <typename T>
inline vec2<T> operator*(T f, vec2<T> v) {
    return vec2<T>(v.x * f, v.y * f);
}
template <typename T>
inline vec2<T> operator*(vec2<T> v, vec2<T> f) {
    return vec2<T>(v.x * f.x, v.y * f.y);
}
template <typename T>
inline vec2<T> operator/(vec2<T> v, T f) {
    return vec2<T>(v.x / f, v.y / f);
}
template <typename T>
inline vec2<T> operator/(T f, vec2<T> v) {
    return vec2<T>(f / v.x, f / v.y);
}
template <typename T>
inline vec2<T> operator/(vec2<T> v, vec2<T> f) {
    return vec2<T>(v.x / f.x, v.y / f.y);
}
template <typename T>
inline bool operator<(vec2<T> v, vec2<T> f) {
    return v.x < f.x && v.y < f.y;
}
template <typename T>
inline bool operator<(vec2<T> v, T f) {
    return v.x < f && v.y < f;
}
template <typename T>
inline bool operator>(vec2<T> v, vec2<T> f) {
    return v.x > f.x && v.y > f.y;
}
template <typename T>
inline bool operator>(vec2<T> v, T f) {
    return v.x > f && v.y > f;
}
template <typename T>
inline bool operator<=(vec2<T> v, vec2<T> f) {
    return v.x <= f.x && v.y <= f.y;
}
template <typename T>
inline bool operator<=(vec2<T> v, T f) {
    return v.x <= f && v.y <= f;
}
template <typename T>
inline bool operator>=(vec2<T> v, vec2<T> f) {
    return v.x >= f.x && v.y >= f.y;
}
template <typename T>
inline bool operator>=(vec2<T> v, T f) {
    return v.x >= f && v.y >= f;
}
template <typename T>
inline bool operator!=(vec2<T> v, vec2<T> f) {
    return f.x != v.x || f.y != v.y;
}
template <typename T>
inline bool operator==(vec2<T> v, vec2<T> f) {
    return f.x == v.x && f.y == v.y;
}
template <typename T>
inline vec2<T> floor(vec2<T> a) {
    return vec2<T>(floor(a.x), floor(a.y));
}
template <typename T>
inline vec2<T> fract(vec2<T> a) {
    return vec2<T>(fract(a.x), fract(a.y));
}
template <typename T>
inline vec2<T> ceil(vec2<T> a) {
    return vec2<T>(ceil(a.x), ceil(a.y));
}
template <typename T>
inline vec2<T> mini(vec2<T> a, vec2<T> b) {
    return vec2<T>(mini(a.x, b.x), mini(a.y, b.y));
}
template <typename T>
inline vec2<T> maxi(vec2<T> a, vec2<T> b) {
    return vec2<T>(maxi(a.x, b.x), maxi(a.y, b.y));
}
template <typename T>
inline T dot(vec2<T> a, vec2<T> b) {
    return a.x * b.x + a.y * b.y;
}
template <typename T>
inline T det(vec2<T> a, vec2<T> b) {
    return a.x * b.y - a.y * b.x;
}
template <typename T>
inline vec2<T> cross(vec2<T> a, vec2<T> b) {
    return vec2<T>(a.x * b.y - a.y * b.x, a.y * b.x - a.x * b.y);
}
template <typename T>
inline vec2<T> reflect(vec2<T> I, vec2<T> N) {
    return I - (T)2 * dotS(N, I) * N;
}
template <typename T>
inline vec2<T> sign(vec2<T> a) {
    return vec2<T>(a.x < (T)0 ? -(T)1 : (T)1, a.y < (T)0 ? (T)-1 : (T)1);
}
template <typename T>
inline vec2<T> sin(vec2<T> a) {
    return vec2<T>(sin<T>(a.x), sin<T>(a.y));
}
template <typename T>
inline vec2<T> cos(vec2<T> a) {
    return vec2<T>(cos<T>(a.x), cos<T>(a.y));
}
template <typename T>
inline vec2<T> tan(vec2<T> a) {
    return vec2<T>(tan<T>(a.x), tan<T>(a.y));
}
template <typename T>
inline vec2<T> atan(vec2<T> a) {
    return vec2<T>(atan<T>(a.x), atan<T>(a.y));
}
using dvec2 = vec2<double>;
using fvec2 = vec2<float>;
using bvec2 = vec2<bool>;
using i8vec2 = vec2<int8_t>;
using i16vec2 = vec2<int16_t>;
using ivec2 = vec2<int32_t>;
using i32vec2 = vec2<int32_t>;
using i64vec2 = vec2<int64_t>;
using u8vec2 = vec2<uint8_t>;
using u16vec2 = vec2<uint16_t>;
using uvec2 = vec2<uint32_t>;
using u32vec2 = vec2<uint32_t>;
using u64vec2 = vec2<uint64_t>;

// convert
inline fvec2 convert(const ivec2& v) {
    return fvec2((float)v.x, (float)v.y);
}
inline ivec2 convert(const fvec2& v) {
    return ivec2((int)v.x, (int)v.y);
}

#ifdef USE_BOX2D
inline b2Vec2 tob2v2(const fvec2& v) {
    return b2Vec2(v.x, v.y);
}
inline b2Vec2 tob2v2(const fvec2& v, const float& r) {
    return b2Vec2(v.x / r, v.y / r);
}
#endif

// specialization for float32 test to fvec2
inline bool valid(const fvec2& a) {
    return floatIsValid(a.x) && floatIsValid(a.y);
}

// specialization for fvec2
inline bool operator==(const fvec2& v, const fvec2& f) {
    return (isFloatEqual(f.x, v.x) && isFloatEqual(f.y, v.y));
}
inline bool operator!=(const fvec2& v, const fvec2& f) {
    return (isFloatDifferent(f.x, v.x) || isFloatDifferent(f.y, v.y));
}

// ne peut focntionner qu'avec double ou float au fond
template <typename T>
inline T prototype_radAngleFromVec2(vec2<T> vec) {
    T angle = 0.0f;
    if (vec.lengthSquared() > T(0) && vec.x != T(0)) {
        angle = atan<T>(vec.y / vec.x);
    }
    return angle;
}

// specialisation
inline float radAngleFromVec2(const fvec2& vec) {
    return prototype_radAngleFromVec2(vec);
}
inline double radAngleFromVec2(const dvec2& vec) {
    return prototype_radAngleFromVec2(vec);
}

// ne peut focntionner qu'avec double ou float au fond
template <typename T>
inline T prototype_radAngleContinuousFromVec2(vec2<T> vec, T angleOffset) {
    T angle = T(0);
    if (vec.x > T(0))
        angle = atan<T>(vec.y / vec.x);
    if (vec.x < T(0))
        angle = _pi - atan<T>(-vec.y / vec.x);
    return angle - angleOffset;
}

// specialisation
inline float radAngleContinuousFromVec2(const fvec2& vec, float angleOffset) {
    return prototype_radAngleContinuousFromVec2(vec, angleOffset);
}
inline double radAngleContinuousFromVec2(const dvec2& vec, double angleOffset) {
    return prototype_radAngleContinuousFromVec2(vec, angleOffset);
}

}  // namespace ez