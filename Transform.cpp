#include "Transform.h"

void Transform::init(int width, int height) {
    float aspect = width * 1.0 / height;
    projection.setPerspective(3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
    this->w = width;
    this->h = height;
    update();
}

void Transform::update() {
    Matrix4x4 worldToView = Matrix4x4::mul(world, view);
    transform = Matrix4x4::mul(worldToView, projection);
}

Vector4D Transform::apply(const Vector4D &x) {
    return Matrix4x4::apply(x, transform);
}

int Transform::checkCVV(const Vector4D &v) {
    int check = 0;
    if (v.z < 0.0f)		check |= 1;
    if (v.z > v.w)		check |= 2;
    if (v.x < -v.w)		check |= 4;
    if (v.x > v.w)		check |= 8;
    if (v.y < -v.w)		check |= 16;
    if (v.y > v.w)		check |= 32;
    return check;
}

Vector4D Transform::homogenize(const Vector4D &x) {
    float rhw = 1.0f / x.w;
    Vector4D ans;
    ans.x = (x.x * rhw + 1.0f) * w * 0.5f;
    ans.y = (1.0f - x.y * rhw) * h * 0.5f;
    ans.z = x.z * rhw;
    ans.w = 1.0f;
    return ans;
}
