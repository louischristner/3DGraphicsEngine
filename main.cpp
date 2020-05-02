/*
** EPITECH PROJECT, 2020
** 3DGraphicsEngine
** File description:
** main
*/

#include <vector>

struct Vect3D {
    float x, y, z;
};

struct Triangle {
    Vect3D p[3];
};

struct Mesh {
    std::vector<Triangle> tris;
};

struct Matrix4x4 {
    float m[4][4] = {0};
};

int main(void)
{
    return (0);
}
