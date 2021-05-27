/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Mesh
*/

#ifndef MESH_HPP_
#define MESH_HPP_

#include <string>
#include <vector>

#include <fstream>
#include <sstream>

#include "Vect3D.hpp"

typedef unsigned char uSmall; // 0 - 255

struct Color {
    uSmall r = 255, g = 255, b = 255;
};

struct Triangle {
    Vect3D p[3];
    Color color;

    float getAverageHeight(void) const;
    std::vector<Triangle> clipAgainstPlane(const Vect3D &plane_p, const Vect3D &plane_n) const;

    friend std::ostream &operator<<(std::ostream &stream, const Triangle &tri)
    {
        stream << tri.p[0] << " " << tri.p[1] << " " << tri.p[2];
        return stream;
    }
};

struct Mesh {
    std::vector<Triangle> tris;

    bool loadFromObjFile(const std::string filename);
};

#endif /* !MESH_HPP_ */
