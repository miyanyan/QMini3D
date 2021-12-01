#ifndef DEVICE_H
#define DEVICE_H

#include "Transform.h"
#include "Trapezoid.h"


/*!
*  渲染设备
*/

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色

class Device
{
public:
    Device(int w = 800, int h = 600);

    Transform transform;							// 坐标变换
    int width;										// 窗口宽
    int height;										// 窗口高
    std::vector<std::vector<uint32_t>> framebuffer;   // 像素缓存
    std::vector<std::vector<float>> zbuffer;        // 深度缓存
    std::vector<std::vector<uint32_t>> texture;       // 纹理
    int tex_width;									// 纹理宽
    int tex_height;									// 纹理高
    float max_u;									// 纹理最大宽
    float max_v;									// 纹理最大高
    int render_state;								// 渲染状态
    uint32_t background;								// 背景颜色
    uint32_t foreground;								// 线框颜色

    // 设置当前纹理
    void setTexture(const std::vector<std::vector<uint32_t>>& bits, int w, int h);

    // 清空 framebuffer and zbuffer
    void clear(int mode);

    // 画点
    void drawPixel(int x, int y, uint32_t color);

    // 绘制线段
    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);

    // 根据坐标读取纹理
    uint32_t readTexture(float u, float v);

    // 绘制扫描线
    void drawScanline(Scanline& scanline);

    // 主渲染函数
    void renderTrap(Trapezoid& trap);

    // 画原始三角形 (primitive 图元) (顶点着色器放在这)
    void drawPrimitive(const Vertex& v1, const Vertex& v2, const Vertex& v3);

private:
    void init();
};

#endif // DEVICE_H
