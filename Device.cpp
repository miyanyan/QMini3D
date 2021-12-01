#include "Device.h"


Device::Device(int w, int h)
    : width(w)
    , height(h)
    , tex_height(2)
    , tex_width(2)
    , max_u(1.0f)
    , max_v(1.0f)
    , render_state(RENDER_STATE_WIREFRAME)
    , background(0xc0c0c0)
    , foreground(0)
{
    init();
}

void Device::setTexture(const std::vector<std::vector<uint32_t> > &bits, int w, int h) {
    texture = bits;
    tex_width = w;
    tex_height = h;
    max_u = (float)(w - 1);
    max_v = (float)(h - 1);
}

void Device::clear(int mode) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            zbuffer[i][j] = 0.0f;
        }
    }
    for (int i = 0; i < height; ++i) {
        uint32_t cc = background;
        if (mode != 0) {
            cc = (height - 1 - i) * 230 / (height - 1);
            cc = (cc << 16) | (cc << 8) | cc; // r g b 分别占 1byte
        }
        for (int j = 0; j < width; ++j) {
            framebuffer[i][j] = cc;
        }
    }
}

void Device::drawPixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    framebuffer[y][x] = color; // 先行后列
}

void Device::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    // 同一个点
    if (x1 == x2 && y1 == y2) {
        drawPixel(x1, y1, color);
    }
    // 竖直
    else if (x1 == x2) {
        int inc = (y1 <= y2) ? 1 : -1;
        for (int y = y1; y != y2; y += inc) {
            drawPixel(x1, y, color);
        }
    }
    // 水平
    else if (y1 == y2) {
        int inc = (x1 <= x2) ? 1 : -1;
        for (int x = x1; x != x2; x += inc) {
            drawPixel(x, y1, color);
        }
    }
    // 斜线
    else {
        int rem = 0;
        int dx = std::abs(x1 - x2);
        int dy = std::abs(y1 - y2);
        if (dx >= dy) {
            if (x2 < x1) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for (int x = x1, y = y1; x <= x2; ++x) {
                drawPixel(x, y, color);
                rem += dy;
                if (rem >= dx) {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    drawPixel(x, y, color);
                }
            }
            drawPixel(x2, y2, color);
        }
        else {
            if (y2 < y1) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for (int x = x1, y = y1; y <= y2; ++y) {
                drawPixel(x, y, color);
                rem += dx;
                if (rem >= dy) {
                    rem -= dy;
                    x += (x2 >= x1) ? 1 : -1;
                    drawPixel(x, y, color);
                }
            }
            drawPixel(x2, y2, color);
        }
    }
}

uint32_t Device::readTexture(float u, float v) {
    u *= max_u;
    v *= max_v;
    int x = (int)(u + 0.5f);
    int y = (int)(v + 0.5f);
    // 裁剪
    x = std::clamp(x, 0, tex_width - 1);
    y = std::clamp(y, 0, tex_height - 1);
    return texture[y][x];
}


/*!
 * \brief Device::drawScanline Edge Walking 第二阶段，按像素迭代 https://www.zhihu.com/question/48299522
 * \param scanline
 */
void Device::drawScanline(Scanline &scanline) {
    auto& fb = framebuffer[scanline.y]; // 当前行的帧缓存
    auto& zb = zbuffer[scanline.y]; // 当前行的深度缓存
    int x = scanline.x;
    int w = scanline.w;
    for (; w > 0; ++x, --w) {
        if (0 <= x && x < width) {
            float rhw = scanline.v.rhw;
            // 深度判断, rhw 越小 w 越大 z 越大
            if (rhw < zb[x]) continue;
            // 更新深度
            zb[x] = rhw;
            float w = 1.0f / rhw;
            if (render_state & RENDER_STATE_COLOR) {
                // 注意之前都除了rhw
                int r = scanline.v.color.r * w * 255;
                int g = scanline.v.color.g * w * 255;
                int b = scanline.v.color.b * w * 255;
                r = std::clamp(r, 0, 255);
                g = std::clamp(g, 0, 255);
                b = std::clamp(b, 0, 255);
                fb[x] = (r << 16) | (g << 8) | b;
            }
            if (render_state & RENDER_STATE_TEXTURE) {
                float u = scanline.v.tc.u * w;
                float v = scanline.v.tc.v * w;
                fb[x] = readTexture(u, v);
            }
        }
        // 增加一个步长
        scanline.v.add(scanline.step);
        if (x >= width) break;
    }
}


/*!
 * \brief Device::renderTrap Edge Walking 第二阶段， 按行迭代
 * \param trap
 */
void Device::renderTrap(Trapezoid &trap) {
    Scanline scanline;
    int top = (int)(trap.top + 0.5f);
    int bottom = (int)(trap.bottom + 0.5f);
    for (int y = top; y < bottom; ++y) {
        // 在屏幕范围内
        if (0 <= y && y < height) {
            trap.edgeInterp((float)y + 0.5f);
            scanline = trap.initScanline(y);
            drawScanline(scanline);
        }
        if (y >= height) break;
    }
}

void Device::drawPrimitive(const Vertex &v1, const Vertex &v2, const Vertex &v3) {
    // MVP 变换
    Point c1 = this->transform.apply(v1.pos);
    Point c2 = this->transform.apply(v2.pos);
    Point c3 = this->transform.apply(v3.pos);
    // 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
    // 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
    if (this->transform.checkCVV(c1) != 0) return;
    if (this->transform.checkCVV(c2) != 0) return;
    if (this->transform.checkCVV(c3) != 0) return;
    // 归一化
    Point p1 = this->transform.homogenize(c1);
    Point p2 = this->transform.homogenize(c2);
    Point p3 = this->transform.homogenize(c3);
    // 纹理或者色彩
    if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
        Vertex t1 = v1, t2 = v2, t3 = v3;

        t1.pos = p1;
        t2.pos = p2;
        t3.pos = p3;
        t1.pos.w = c1.w;
        t2.pos.w = c2.w;
        t3.pos.w = c3.w;

        t1.init();
        t2.init();
        t3.init();

        // 三角形拆为梯形
        auto traps = Trapezoid::initTriangle(t1, t2, t3);
        for (auto& trap : traps) {
            renderTrap(trap);
        }
    }
    // 线框
    if (render_state & RENDER_STATE_WIREFRAME) {
        drawLine(p1.x, p1.y, p2.x, p2.y, this->foreground);
        drawLine(p1.x, p1.y, p3.x, p3.y, this->foreground);
        drawLine(p3.x, p3.y, p2.x, p2.y, this->foreground);
    }
}

void Device::init() {
    framebuffer.resize(height, std::vector<uint32_t>(width));
    zbuffer.resize(height, std::vector<float>(width));
    texture.resize(height, std::vector<uint32_t>(width));
    transform.init(width, height);
}
