#pragma once

namespace ez {
    	
template <typename T>
struct quat {
    // x,y,z => axis
    // w => angle
    T x, y, z, w;
    quat() : x((T)0), y((T)0), z((T)0), w((T)1) {
    }
    quat(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {
    }
    quat(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {
    }
    T getTx() {
        return x * w;
    }
    T getTy() {
        return y * w;
    }
    T getTz() {
        return z * w;
    }
    void add(const quat<T>& vQuat) {
        x += vQuat.x;
        y += vQuat.y;
        z += vQuat.z;
        w += vQuat.w;
    }
    void sub(const quat<T>& vQuat) {
        x -= vQuat.x;
        y -= vQuat.y;
        z -= vQuat.z;
        w -= vQuat.w;
    }
    void mul(const quat<T>& vQuat) {
        const T tx = x, ty = y, tz = z, tw = w;
        x = tx * vQuat.w + ty * vQuat.z - tz * vQuat.y + tw * vQuat.x;
        y = -tx * vQuat.z + ty * vQuat.w + tz * vQuat.x + tw * vQuat.y;
        z = tx * vQuat.y - ty * vQuat.x + tz * vQuat.w + tw * vQuat.z;
        w = -tx * vQuat.x - ty * vQuat.y - tz * vQuat.z + tw * vQuat.w;
    }
    void conjugate(const quat<T>& vQuat) {
        x = -vQuat.x;
        y = -vQuat.y;
        z = -vQuat.z;
        w = vQuat.w;
    }
    void normalize() {
        assert(0);  // not supported by other types thant float and double
    }
    void scale(T s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
    }
};
typedef quat<float> fquat;
typedef quat<double> dquat;

}  // namespace ez
