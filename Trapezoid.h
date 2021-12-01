#ifndef TRAPEZOID_H
#define TRAPEZOID_H


#include "Vertex.h"


// 边
struct Edge
{
    Vertex v, v1, v2;
};

// 扫描线
struct Scanline
{
    Vertex v, step;
    int x, y, w;
};

// 梯形
class Trapezoid
{
public:
    float top, bottom;
    Edge left, right;

    // 按照 y 计算左右两条边纵坐标等于 y 的顶点
    void edgeInterp(float y);

    // 根据左右两边的端点，初始化计算出扫描线的起点和步长
    Scanline initScanline(int y);

    /*!
    * static function
    */
    // 根据三角形生成 0-2 个梯形
    static std::vector<Trapezoid> initTriangle(Vertex& p1, Vertex& p2, Vertex& p3);
};

#endif // TRAPEZOID_H
