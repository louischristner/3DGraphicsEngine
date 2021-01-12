/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Matrix4x4
*/

#include "../headers/Matrix4x4.hpp"

Vect3D Matrix4x4::operator*(const Vect3D &v) const
{
    float w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3];
    Vect3D vector = {
        v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
        v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
        v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]
    };

    if (w != 0.0f) {
        vector /= w;
    }

    return vector;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &m2) const
{
    Matrix4x4 matrix;

    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m[r][0] * m2.m[0][c] + m[r][1] * m2.m[1][c] + m[r][2] * m2.m[2][c] + m[r][3] * m2.m[3][c];
    return matrix;
}

Matrix4x4 Matrix4x4::createIdentity(void)
{
    Matrix4x4 matrix;

    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

Matrix4x4 Matrix4x4::createRotationX(float fAngleRad)
{
    Matrix4x4 matrix;

    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

Matrix4x4 Matrix4x4::createRotationY(float fAngleRad)
{
    Matrix4x4 matrix;

    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}


Matrix4x4 Matrix4x4::createRotationZ(float fAngleRad)
{
    Matrix4x4 matrix;

    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

Matrix4x4 Matrix4x4::createTranslation(float x, float y, float z)
{
    Matrix4x4 matrix = Matrix4x4::createIdentity();

    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

Matrix4x4 Matrix4x4::createProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
    Matrix4x4 matrix;
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * M_PI);

    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / (fFar - fNear);
    matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}
