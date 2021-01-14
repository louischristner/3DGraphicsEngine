/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Matrix4x4
*/

#include "../headers/Matrix4x4.hpp"

/**
 * Quick implementation of matrices inverser
 *
 * @return the inverse of the matrix
 * @deprecated this is only for rotation & translation matrices
 */
Matrix4x4 Matrix4x4::quickInverse(void) const noexcept
{
    Matrix4x4 matrix;

    matrix.m[0][0] = m[0][0];
    matrix.m[0][1] = m[1][0];
    matrix.m[0][2] = m[2][0];
    matrix.m[0][3] = 0.0f;

    matrix.m[1][0] = m[0][1];
    matrix.m[1][1] = m[1][1];
    matrix.m[1][2] = m[2][1];
    matrix.m[1][3] = 0.0f;

    matrix.m[2][0] = m[0][2];
    matrix.m[2][1] = m[1][2];
    matrix.m[2][2] = m[2][2];
    matrix.m[2][3] = 0.0f;

    matrix.m[3][0] = -(m[3][0] * matrix.m[0][0] + m[3][1] * matrix.m[1][0] + m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m[3][0] * matrix.m[0][1] + m[3][1] * matrix.m[1][1] + m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m[3][0] * matrix.m[0][2] + m[3][1] * matrix.m[1][2] + m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;

    return matrix;
}

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

Matrix4x4 Matrix4x4::createPointAt(const Vect3D &pos, const Vect3D &target, const Vect3D &up)
{
    // Calculate new forward direction
    Vect3D newForward = (target - pos).normalize();

    // Calculate new up direction
    Vect3D newUp = (up - (newForward * up.dotProduct(newForward))).normalize();

    // Calculate new right direction
    Vect3D newRight = newUp.crossProduct(newForward);

    Matrix4x4 matrix;

    matrix.m[0][0] = newRight.x;
    matrix.m[0][1] = newRight.y;
    matrix.m[0][2] = newRight.z;
    matrix.m[0][3] = .0f;

    matrix.m[1][0] = newUp.x;
    matrix.m[1][1] = newUp.y;
    matrix.m[1][2] = newUp.z;
    matrix.m[1][3] = .0f;

    matrix.m[2][0] = newForward.x;
    matrix.m[2][1] = newForward.y;
    matrix.m[2][2] = newForward.z;
    matrix.m[2][3] = .0f;

    matrix.m[3][0] = pos.x;
    matrix.m[3][1] = pos.y;
    matrix.m[3][2] = pos.z;
    matrix.m[3][3] = 1.0f;

    return matrix;
}
