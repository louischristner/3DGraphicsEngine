/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Vect3D
*/

#include "../headers/Vect3D.hpp"

float Vect3D::length(void) const noexcept
{
    return sqrtf(x * x + y * y + z * z);
}

Vect3D Vect3D::normalize(void) const noexcept
{
    return ((*this) / length());
}

Vect3D Vect3D::crossProduct(const Vect3D &v) const noexcept
{
    return {
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    };
}

// float Vect3D::dot(const Vect3D &v) const noexcept
// {
//     return acosf((x * v.x + y * v.y + z * v.z) / (length() * v.length()));
// }

float Vect3D::dotProduct(const Vect3D &v) const noexcept
{
    return (x * v.x + y * v.y + z * v.z);
}

void Vect3D::operator+=(const float &n)
{
    x += n;
    y += n;
    z += n;
}

void Vect3D::operator+=(const Vect3D &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vect3D::operator-=(const float &n)
{
    x -= n;
    y -= n;
    z -= n;
}

void Vect3D::operator-=(const Vect3D &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vect3D::operator*=(const float &n)
{
    x *= n;
    y *= n;
    z *= n;
}

void Vect3D::operator*=(const Vect3D &v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}

void Vect3D::operator/=(const float &n)
{
    x /= n;
    y /= n;
    z /= n;
}

void Vect3D::operator/=(const Vect3D &v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

Vect3D Vect3D::operator+(const float &n) const
{
    return {
        x + n,
        y + n,
        z + n
    };
}

Vect3D Vect3D::operator+(const Vect3D &v) const
{
    return {
        x + v.x,
        y + v.y,
        z + v.z
    };
}

Vect3D Vect3D::operator-(const float &n) const
{
    return {
        x - n,
        y - n,
        z - n
    };
}

Vect3D Vect3D::operator-(const Vect3D &v) const
{
    return {
        x - v.x,
        y - v.y,
        z - v.z
    };
}

Vect3D Vect3D::operator*(const float &n) const
{
    return {
        x * n,
        y * n,
        z * n
    };
}

Vect3D Vect3D::operator*(const Vect3D &v) const
{
    return {
        x * v.x,
        y * v.y,
        z * v.z
    };
}

Vect3D Vect3D::operator/(const float &n) const
{
    return {
        x / n,
        y / n,
        z / n
    };
}

Vect3D Vect3D::operator/(const Vect3D &v) const
{
    return {
        x / v.x,
        y / v.y,
        z / v.z
    };
}

Vect3D Vect3D::intersectPlane(const Vect3D &plane_p, const Vect3D &plane_n, const Vect3D &lineStart, const Vect3D &lineEnd)
{
    Vect3D normalizedPlane_n = plane_n.normalize();
    float plane_d = -normalizedPlane_n.dotProduct(plane_p);
    float ad = lineStart.dotProduct(normalizedPlane_n);
    float bd = lineEnd.dotProduct(normalizedPlane_n);
    float t = (-plane_d - ad) / (bd - ad);
    Vect3D lineStartToEnd = lineEnd - lineStart;
    Vect3D lineToIntersect = lineStartToEnd * t;

    return lineStart + lineToIntersect;
}
