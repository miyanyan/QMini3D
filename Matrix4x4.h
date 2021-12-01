#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector4D.h"


class Matrix4x4
{
public:
    Matrix4x4();
    float m[4][4];
    // 设为单位矩阵
    void setToIdentity();
    // 设为零矩阵
    void setToZero();
    // 缩放
    void scale(float f);
    // 平移变换
    void setTranslate(float x, float y, float z);
    // 缩放变换
    void setScale(float x, float y, float z);
    // 旋转矩阵, 四元数 https://krasjet.github.io/quaternion/quaternion.pdf
    void setRotate(float x, float y, float z, float theta);
    // 设置摄像机, https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/
    void setLoockat(const Vector4D& eye, const Vector4D& at, const Vector4D& up);
    // 透视投影
    void setPerspective(float fovy, float aspect, float znear, float zfar);

    /*!
    * 静态函数
    */
    static Matrix4x4 add(const Matrix4x4& m1, const Matrix4x4& m2);
    //
    static Matrix4x4 sub(const Matrix4x4& m1, const Matrix4x4& m2);
    //
    static Matrix4x4 mul(const Matrix4x4& m1, const Matrix4x4& m2);
    // 向量乘
    static Vector4D apply(const Vector4D& v, const Matrix4x4& m);
};

#endif // MATRIX4X4_H
