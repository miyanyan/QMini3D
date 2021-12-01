#include "Vertex.h"

void Vertex::init() {
    // 为什么要除以 w(相机空间的z)? https://blog.csdn.net/popy007/article/details/5570803
    // 纹理 u v 与 屏幕坐标 x y 不是线性关系
    // u/w v/w 与 屏幕坐标 x y 是线性关系
    rhw = 1.0f / pos.w;
    tc.u *= rhw;
    tc.v *= rhw;
    color.r *= rhw;
    color.g *= rhw;
    color.b *= rhw;
}

void Vertex::add(const Vertex &x) {
    this->pos.x += x.pos.x;
    this->pos.y += x.pos.y;
    this->pos.z += x.pos.z;
    this->pos.w += x.pos.w;
    this->rhw += x.rhw;
    this->tc.u += x.tc.u;
    this->tc.v += x.tc.v;
    this->color.r += x.color.r;
    this->color.g += x.color.g;
    this->color.b += x.color.b;
}

Vertex Vertex::interp(const Vertex &x1, const Vertex &x2, float t) {
    Vertex ans;
    ans.pos = Vector4D::interp(x1.pos, x2.pos, t);
    ans.tc.u = interp1D(x1.tc.u, x2.tc.u, t);
    ans.tc.v = interp1D(x1.tc.v, x2.tc.v, t);
    ans.color.r = interp1D(x1.color.r, x2.color.r, t);
    ans.color.g = interp1D(x1.color.g, x2.color.g, t);
    ans.color.b = interp1D(x1.color.b, x2.color.b, t);
    ans.rhw = interp1D(x1.rhw, x2.rhw, t);
    return ans;
}

Vertex Vertex::division(const Vertex &x1, const Vertex &x2, float w) {
    float inv = 1.0f / w;
    Vertex ans;
    ans.pos.x = (x2.pos.x - x1.pos.x) * inv;
    ans.pos.y = (x2.pos.y - x1.pos.y) * inv;
    ans.pos.z = (x2.pos.z - x1.pos.z) * inv;
    ans.pos.w = (x2.pos.w - x1.pos.w) * inv;
    ans.tc.u = (x2.tc.u - x1.tc.u) * inv;
    ans.tc.v = (x2.tc.v - x1.tc.v) * inv;
    ans.color.r = (x2.color.r - x1.color.r) * inv;
    ans.color.g = (x2.color.g - x1.color.g) * inv;
    ans.color.b = (x2.color.b - x1.color.b) * inv;
    ans.rhw = (x2.rhw - x1.rhw) * inv;
    return ans;
}
