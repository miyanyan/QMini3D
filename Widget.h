#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <unordered_map>
#include "Device.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private:
    /*!
    *  画平面
    */
    void draw_plane(Device& device, int a, int b, int c, int d);

    /*!
    *  画立方体
    */
    void draw_cube(Device& device, float theta);

    void camera_at_zero(Device& device, float x, float y, float z);

    void init_texture(Device& device);

    // 环形链表
    std::vector<int> m_renderStateList;
    int m_renderStateIndex;
    // 存储按下的按键
    std::unordered_map<int, bool> m_keyPressed;

    Device m_renderDevice;
    float m_pos;
    float m_alpha;
};
#endif // WIDGET_H
