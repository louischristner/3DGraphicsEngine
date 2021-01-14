/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Matrix4x4
*/

#ifndef MATRIX4X4_HPP_
#define MATRIX4X4_HPP_

#include <cmath>

#include "Vect3D.hpp"

struct Matrix4x4 {
    float m[4][4] = {0};

    Matrix4x4 quickInverse(void) const noexcept;

    Vect3D operator*(const Vect3D &v) const;
    Matrix4x4 operator*(const Matrix4x4 &m2) const;

    static Matrix4x4 createIdentity(void);
    static Matrix4x4 createRotationX(float fAngleRad);
    static Matrix4x4 createRotationY(float fAngleRad);
    static Matrix4x4 createRotationZ(float fAngleRad);
    static Matrix4x4 createTranslation(float x, float y, float z);
    static Matrix4x4 createProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
    static Matrix4x4 createPointAt(const Vect3D &pos, const Vect3D &target, const Vect3D &up);
};

#endif /* !MATRIX4X4_HPP_ */
