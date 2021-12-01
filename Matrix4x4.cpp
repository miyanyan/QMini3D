#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
    setToIdentity();
}

void Matrix4x4::setToIdentity() {
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

void Matrix4x4::setToZero() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m[i][j] = 0.0f;
        }
    }
}

void Matrix4x4::scale(float f) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m[i][j] *= f;
        }
    }
}

void Matrix4x4::setTranslate(float x, float y, float z) {
    setToIdentity();
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
}

void Matrix4x4::setScale(float x, float y, float z) {
    setToIdentity();
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
}

void Matrix4x4::setRotate(float x, float y, float z, float theta) {
    // 旋转轴 u
    Vector4D u(x, y, z, 1.0f);
    u.normalize();
    // 令 w = cos(θ * 0.5） x = sin((θ * 0.5) * u.x
    float qsin = std::sin(theta * 0.5f);
    float qcos = std::cos(theta * 0.5f);
    float w = qcos;
    x = u.x * qsin;
    y = u.y * qsin;
    z = u.z * qsin;

    m[0][0] = 1 - 2 * y * y - 2 * z * z;
    m[1][0] = 2 * x * y - 2 * w * z;
    m[2][0] = 2 * x * z + 2 * w * y;
    m[0][1] = 2 * x * y + 2 * w * z;
    m[1][1] = 1 - 2 * x * x - 2 * z * z;
    m[2][1] = 2 * y * z - 2 * w * x;
    m[0][2] = 2 * x * z - 2 * w * y;
    m[1][2] = 2 * y * z + 2 * w * x;
    m[2][2] = 1 - 2 * x * x - 2 * y * y;
    m[0][3] = m[1][3] = m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

void Matrix4x4::setLoockat(const Vector4D &eye, const Vector4D &at, const Vector4D &up) {
    Vector4D zaxis = Vector4D::sub(at, eye);
    zaxis.normalize();
    Vector4D xaxis = Vector4D::crossproduct(up, zaxis);
    xaxis.normalize();
    Vector4D yaxis = Vector4D::crossproduct(zaxis, xaxis);
    // 右向量
    m[0][0] = xaxis.x;
    m[1][0] = xaxis.y;
    m[2][0] = xaxis.z;
    m[3][0] = -Vector4D::dotproduct(xaxis, eye);
    // 上向量
    m[0][1] = yaxis.x;
    m[1][1] = yaxis.y;
    m[2][1] = yaxis.z;
    m[3][1] = -Vector4D::dotproduct(yaxis, eye);
    // 方向向量
    m[0][2] = zaxis.x;
    m[1][2] = zaxis.y;
    m[2][2] = zaxis.z;
    m[3][2] = -Vector4D::dotproduct(zaxis, eye);

    m[0][3] = m[1][3] = m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

void Matrix4x4::setPerspective(float fovy, float aspect, float znear, float zfar) {
    setToIdentity();
    float fax = 1.0f / (float)std::tan(fovy * 0.5f);
    m[0][0] = (float)(fax / aspect);
    m[1][1] = (float)(fax);
    m[2][2] = zfar / (zfar - znear);
    m[3][2] = -znear * zfar / (zfar - znear);
    m[2][3] = 1;
}

Matrix4x4 Matrix4x4::add(const Matrix4x4 &m1, const Matrix4x4 &m2) {
    Matrix4x4 ans;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ans.m[i][j] = m1.m[i][j] + m2.m[i][j];
        }
    }
    return ans;
}

Matrix4x4 Matrix4x4::sub(const Matrix4x4 &m1, const Matrix4x4 &m2) {
    Matrix4x4 ans;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ans.m[i][j] = m1.m[i][j] - m2.m[i][j];
        }
    }
    return ans;
}

Matrix4x4 Matrix4x4::mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {
    Matrix4x4 ans;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ans.m[i][j] = m1.m[i][0] * m2.m[0][j]
                          + m1.m[i][1] * m2.m[1][j]
                          + m1.m[i][2] * m2.m[2][j]
                          + m1.m[i][3] * m2.m[3][j];
        }
    }
    return ans;
}

Vector4D Matrix4x4::apply(const Vector4D &v, const Matrix4x4 &m) {
    float X, Y, Z, W;
    X = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
    Y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
    Z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
    W = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
    return Vector4D(X, Y, Z, W);
}
