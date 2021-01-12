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
    uSmall r, g, b;
};

struct Triangle {
    Vect3D p[3];
    Color color;
};

struct Mesh {
    std::vector<Triangle> tris;

    bool loadFromObjFile(const std::string filename);
};

#endif /* !MESH_HPP_ */
