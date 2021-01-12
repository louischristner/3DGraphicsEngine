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
