#ifndef TRANSFORM_H
#define TRANSFORM_H


#include "Matrix4x4.h"


/*!
* 坐标变换
*/
class Transform
{
public:
    Matrix4x4 world;
    Matrix4x4 view;
    Matrix4x4 projection;
    Matrix4x4 transform;	// transform = world * view * projection
    float w, h;				// 屏幕 宽 高

    // 初始化
    void init(int width, int height);
    // 更新变换矩阵
    void update();
    // 计算矢量的project
    Vector4D apply(const Vector4D& x);
    // 检查齐次坐标同 cvv 的边界用于视锥裁剪
    int checkCVV(const Vector4D& v);
    /*!
    * 归一化，得到屏幕坐标,
    * 原点移动到屏幕中心，y轴翻转
    */
    Vector4D homogenize(const Vector4D& x);
};


#endif // TRANSFORM_H
