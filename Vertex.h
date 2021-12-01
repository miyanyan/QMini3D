#ifndef VERTEX_H
#define VERTEX_H

#include "Vector4D.h"

/*!
* 几何计算：顶点、扫描线、边缘、矩形、步长计算
*/
// 点
using Point = Vector4D;
// 颜色
struct Color
{
    float r, g, b;
};
// 纹理坐标
struct Texcoord
{
    float u, v;
};
// 顶点
class Vertex
{
public:
    Vertex() {}
    Vertex(Point pos_, Texcoord tc_, Color color_, float rhw_)
        : pos(pos_)
        , tc(tc_)
        , color(color_)
        , rhw(rhw_)
    {}

    Point pos;
    Texcoord tc;
    Color color;
    float rhw;

    void init();

    void add(const Vertex& x);

    /*!
    * 静态函数
    */
    static Vertex interp(const Vertex& x1, const Vertex& x2, float t);

    static Vertex division(const Vertex& x1, const Vertex& x2, float w);
};

#endif // VERTEX_H
