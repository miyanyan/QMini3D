#include "Trapezoid.h"

void Trapezoid::edgeInterp(float y) {
    float s1 = this->left.v2.pos.y - this->left.v1.pos.y;
    float s2 = this->right.v2.pos.y - this->right.v1.pos.y;
    float t1 = (y - this->left.v1.pos.y) / s1;
    float t2 = (y - this->right.v1.pos.y) / s2;
    this->left.v = Vertex::interp(this->left.v1, this->left.v2, t1);
    this->right.v = Vertex::interp(this->right.v1, this->right.v2, t2);
}

Scanline Trapezoid::initScanline(int y) {
    Scanline ans;
    ans.x = (int)(this->left.v.pos.x + 0.5f);				// 起点横坐标
    ans.w = (int)(this->right.v.pos.x + 0.5f) - ans.x;		// 长度
    ans.y = y;												// 起点纵坐标
    ans.v = this->left.v;									// 起点（颜色、uv）

    float width = this->right.v.pos.x - this->left.v.pos.x; // 长度
    if (width <= 0) {
        ans.w = 0;
    }
    ans.step = Vertex::division(this->left.v, this->right.v, width);
    return ans;
}

std::vector<Trapezoid> Trapezoid::initTriangle(Vertex &p1, Vertex &p2, Vertex &p3) {
    // y轴向下，p1.y < p2.y < p3.y
    if (p1.pos.y > p2.pos.y) std::swap(p1, p2);
    if (p1.pos.y > p3.pos.y) std::swap(p1, p3);
    if (p2.pos.y > p3.pos.y) std::swap(p2, p3);
    // 三点一线
    if (p1.pos.y == p2.pos.y && p2.pos.y == p3.pos.y) return {};
    if (p1.pos.x == p2.pos.x && p2.pos.x == p3.pos.x) return {};

    std::vector<Trapezoid> ans;
    // 底部朝上的三角形(y轴向下）
    if (p1.pos.y == p2.pos.y) {
        if (p1.pos.x > p2.pos.x) std::swap(p1, p2);
        Trapezoid trap;
        trap.top = p1.pos.y;
        trap.bottom = p3.pos.y;
        trap.left.v1 = p1;
        trap.left.v2 = p3;
        trap.right.v1 = p2;
        trap.right.v2 = p3;
        ans.push_back(trap);
        return ans;
    }
    // 底部朝下的三角形(y轴向下）
    if (p2.pos.y == p3.pos.y) {
        if (p2.pos.x > p3.pos.x) std::swap(p2, p3);
        Trapezoid trap;
        trap.top = p1.pos.y;
        trap.bottom = p3.pos.y;
        trap.left.v1 = p1;
        trap.left.v2 = p2;
        trap.right.v1 = p1;
        trap.right.v2 = p3;
        ans.push_back(trap);
        return ans;
    }
    // 一定会生成两个
    ans.resize(2);
    ans[0].top = p1.pos.y;
    ans[0].bottom = p2.pos.y;
    ans[1].top = p2.pos.y;
    ans[1].bottom = p3.pos.y;

    float k = (p3.pos.y - p1.pos.y) / (p2.pos.y - p1.pos.y);
    float x = p1.pos.x + (p2.pos.x - p1.pos.x) * k;
    /*      p1                p1
        *	   /  \                \  \
        *	  p2   \                \   p2
        *	 /		\                \    \
        *   x        p3               p3    x
        */
    if (x <= p3.pos.x) {
        ans[0].left.v1 = p1;
        ans[0].left.v2 = p2;
        ans[0].right.v1 = p1;
        ans[0].right.v2 = p3;
        ans[1].left.v1 = p2;
        ans[1].left.v2 = p3;
        ans[1].right.v1 = p1;
        ans[1].right.v2 = p3;
    }
    else {
        ans[0].left.v1 = p1;
        ans[0].left.v2 = p3;
        ans[0].right.v1 = p1;
        ans[0].right.v2 = p2;
        ans[1].left.v1 = p1;
        ans[1].left.v2 = p3;
        ans[1].right.v1 = p2;
        ans[1].right.v2 = p3;
    }
    return ans;
}
