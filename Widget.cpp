#include "Widget.h"
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>


Vertex mesh[8] = {
    { Point{ -1, -1,  1, 1 }, Texcoord{ 0, 0 }, Color{ 1.0f, 0.2f, 0.2f }, 1 },
    { Point{  1, -1,  1, 1 }, Texcoord{ 0, 1 }, Color{ 0.2f, 1.0f, 0.2f }, 1 },
    { Point{  1,  1,  1, 1 }, Texcoord{ 1, 1 }, Color{ 0.2f, 0.2f, 1.0f }, 1 },
    { Point{ -1,  1,  1, 1 }, Texcoord{ 1, 0 }, Color{ 1.0f, 0.2f, 1.0f }, 1 },
    { Point{ -1, -1, -1, 1 }, Texcoord{ 0, 0 }, Color{ 1.0f, 1.0f, 0.2f }, 1 },
    { Point{  1, -1, -1, 1 }, Texcoord{ 0, 1 }, Color{ 0.2f, 1.0f, 1.0f }, 1 },
    { Point{  1,  1, -1, 1 }, Texcoord{ 1, 1 }, Color{ 1.0f, 0.3f, 0.3f }, 1 },
    { Point{ -1,  1, -1, 1 }, Texcoord{ 1, 0 }, Color{ 0.2f, 1.0f, 0.3f }, 1 },
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_pos(4.0f)
    , m_alpha(1.0f)
    , m_renderStateList{1, 2, 3, 4, 5, 6, 7}
    , m_renderStateIndex(0)
{
    int w = 800, h = 600;
    this->setFixedSize(w, h);
    this->setWindowTitle("Mini3D : Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

    m_renderDevice = Device(w, h);
    camera_at_zero(m_renderDevice, 3, 0, 0);
    init_texture(m_renderDevice);
    m_renderDevice.render_state = m_renderStateList[m_renderStateIndex];

    // 键盘响应慢，改为定时刷新
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&](){
        if (m_keyPressed[Qt::Key_Up] || m_keyPressed[Qt::Key_Down] || m_keyPressed[Qt::Key_Left] || m_keyPressed[Qt::Key_Right]) {
            this->update();
        }
    });
    timer->start(1);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *e)
{
    // 键盘响应
    if (m_keyPressed[Qt::Key_Up]) {
        m_pos -= 0.1f;
    }
    if (m_keyPressed[Qt::Key_Down]) {
        m_pos += 0.1f;
    }
    if (m_keyPressed[Qt::Key_Left]) {
        m_alpha += 0.1f;
    }
    if (m_keyPressed[Qt::Key_Right]) {
        m_alpha -= 0.1f;
    }

    m_renderDevice.clear(1);
    camera_at_zero(m_renderDevice, m_pos, 0, 0); // m_pos > 0 从右向原点看，此时正方体右面在屏幕空间为正面
    draw_cube(m_renderDevice, m_alpha);
    QImage image(this->width(), this->height(), QImage::Format_RGB888);
    for (int i = 0; i < this->height(); ++i) {
        for (int j = 0; j < this->width(); ++j) {
            auto color = m_renderDevice.framebuffer[i][j];
            int r = color >> 16;
            int g = (color >> 8) ^ (r << 8);
            int b = color ^ (r << 16) ^ (g << 8);
            image.setPixelColor(j, i, QColor(r, g, b));
        }
    }
    QPainter painter(this);
    painter.drawImage(this->rect(), image);
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    // 空格切换模式
    if (e->key() == Qt::Key_Space) {
        m_renderStateIndex++;
        if (m_renderStateIndex >= m_renderStateList.size()) {
            m_renderStateIndex = 0;
        }
        m_renderDevice.render_state = m_renderStateList[m_renderStateIndex];
        update();
    }
    // 键盘
    else if (e->key() == Qt::Key_Up ||
        e->key() == Qt::Key_Down ||
        e->key() == Qt::Key_Left ||
        e->key() == Qt::Key_Right) {
        m_keyPressed[e->key()] = true;
    }
}

void Widget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up ||
        e->key() == Qt::Key_Down ||
        e->key() == Qt::Key_Left ||
        e->key() == Qt::Key_Right) {
        m_keyPressed[e->key()] = false;
    }
}

void Widget::draw_plane(Device &device, int a, int b, int c, int d) {
    Vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
    p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
    p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
    device.drawPrimitive(p1, p2, p3);
    device.drawPrimitive(p3, p4, p1);
}

void Widget::draw_cube(Device &device, float theta) {
    device.transform.world.setRotate(-1, -0.5, 1, theta); // 旋转轴, 更改world
    device.transform.update();

    draw_plane(device, 0, 1, 2, 3); // 正面
    draw_plane(device, 7, 6, 5, 4); // 背面
    draw_plane(device, 0, 4, 5, 1); // 底面
    draw_plane(device, 1, 5, 6, 2); // 右面
    draw_plane(device, 2, 6, 7, 3); // 上面
    draw_plane(device, 3, 7, 4, 0); // 左面
}

void Widget::camera_at_zero(Device &device, float x, float y, float z) {
    Point eye{ x, y, z, 1 };
    Point at{ 0, 0, 0, 1 };
    Point up{ 0, 0, 1, 1 };

    device.transform.view.setLoockat(eye, at, up); // 更改view
    device.transform.update();
}

void Widget::init_texture(Device &device) {
    int tex_size = 256;
    std::vector<std::vector<uint32_t>> texture(tex_size, std::vector<uint32_t>(tex_size));
    for (int i = 0; i < tex_size; ++i) {
        for (int j = 0; j < tex_size; ++j) {
            int x = i / 32, y = j / 32;
            texture[i][j] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
        }
    }
    device.setTexture(texture, tex_size, tex_size);
}

