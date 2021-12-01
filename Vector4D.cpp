#include "Vector4D.h"

Vector4D::Vector4D(float x_, float y_, float z_, float w_)
    : x(x_)
    , y(y_)
    , z(z_)
    , w(w_)
{

}

float Vector4D::length() {
    float sq = x * x + y * y + z * z;
    return (float)std::sqrt(sq);
}

void Vector4D::normalize() {
    float len = length();
    if (len != 0.0f) {
        float inv = 1.0f / len;
        x *= inv;
        y *= inv;
        z *= inv;
    }
}

Vector4D Vector4D::add(const Vector4D &v1, const Vector4D &v2) {
    return Vector4D(v1.x + v2.x, v1.y + v2.y, v1.x + v2.y, 1.0f);
}

Vector4D Vector4D::sub(const Vector4D &v1, const Vector4D &v2) {
    return Vector4D(v1.x - v2.x, v1.y - v2.y, v1.x - v2.y, 1.0f);
}

float Vector4D::dotproduct(const Vector4D &v1, const Vector4D &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector4D Vector4D::crossproduct(const Vector4D &v1, const Vector4D &v2) {
    float m1, m2, m3;
    m1 = v1.y * v2.z - v1.z * v2.y;
    m2 = v1.z * v2.x - v1.x * v2.z;
    m3 = v1.x * v2.y - v1.y * v2.x;
    return Vector4D(m1, m2, m3, 1.0f);
}

Vector4D Vector4D::interp(const Vector4D &v1, const Vector4D &v2, float t) {
    float m1, m2, m3;
    m1 = interp1D(v1.x, v2.x, t);
    m2 = interp1D(v1.y, v2.y, t);
    m3 = interp1D(v1.z, v2.z, t);
    return Vector4D(m1, m2, m3, 1.0f);
}
