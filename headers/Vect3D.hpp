/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Vect3D
*/

#ifndef VECT3D_HPP_
#define VECT3D_HPP_

#include <cmath>

#include <iostream>

struct Vect3D
{
    float x, y, z;

    float length(void) const noexcept;
    Vect3D normalize(void) const noexcept;
    Vect3D crossProduct(const Vect3D &v) const noexcept;
    float dotProduct(const Vect3D &v) const noexcept;

    void operator+=(const float &n);
    void operator+=(const Vect3D &v);
    void operator-=(const float &n);
    void operator-=(const Vect3D &v);
    void operator*=(const float &n);
    void operator*=(const Vect3D &v);
    void operator/=(const float &n);
    void operator/=(const Vect3D &v);

    Vect3D operator+(const float &n) const;
    Vect3D operator+(const Vect3D &v) const;
    Vect3D operator-(const float &n) const;
    Vect3D operator-(const Vect3D &v) const;
    Vect3D operator*(const float &n) const;
    Vect3D operator*(const Vect3D &v) const;
    Vect3D operator/(const float &n) const;
    Vect3D operator/(const Vect3D &v) const;

    static Vect3D intersectPlane(const Vect3D &plane_p, const Vect3D &plane_n, const Vect3D &lineStart, const Vect3D &lineEnd);

    friend std::ostream &operator<<(std::ostream &stream, const Vect3D &vect)
    {
        stream << "(" << vect.x << ", " << vect.y << ", " << vect.z << ")";
        return stream;
    }
};

#endif /* !VECT3D_HPP_ */
