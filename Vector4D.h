#ifndef VECTOR4D_H
#define VECTOR4D_H

#include <algorithm>
#include <vector>

/*!
* 数学库
*/
// 计算插值 t -> [0, 1]
static float interp1D(float x1, float x2, float t) {
    return x1 + (x2 - x1) * t;
}


class Vector4D
{
public:
    Vector4D (float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 1.0f);
    float x, y, z, w;
    // | v |
    float length();
    // 归一化
    void normalize();
    /*!
    * 静态函数
    */
    static Vector4D add(const Vector4D& v1, const Vector4D& v2);

    static Vector4D sub(const Vector4D& v1, const Vector4D& v2);
    // 点乘
    static float dotproduct(const Vector4D& v1, const Vector4D& v2);
    // 叉乘
    static Vector4D crossproduct(const Vector4D& v1, const Vector4D& v2);
    // 矢量插值
    static Vector4D interp(const Vector4D& v1, const Vector4D& v2, float t);
};

#endif // VECTOR4D_H
