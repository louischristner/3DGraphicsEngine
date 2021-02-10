/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Mesh
*/

#include "../headers/Mesh.hpp"

std::vector<Triangle> Triangle::clipAgainstPlane(const Vect3D &plane_p, const Vect3D &plane_n) const
{
    std::vector<Triangle> validTriangles;
    Vect3D normalizedPlane_n = plane_n.normalize();

    auto dist = [normalizedPlane_n, plane_p](const Vect3D &point)
    {
        // Vect3D n = p.normalize();
        return (normalizedPlane_n.x * point.x + normalizedPlane_n.y * point.y + normalizedPlane_n.z * point.z - normalizedPlane_n.dotProduct(plane_p));
    };

    Vect3D const *insidePoints[3];
    Vect3D const *outsidePoints[3];
    int nInsidePointCount = 0;
    int nOutsidePointCount = 0;

    // Iterate through the points of the triangle
    for (int i = 0; i < 3; i++) {
        if (dist(p[i]) >= 0) {
            insidePoints[nInsidePointCount++] = &(p[i]);
        } else {
            outsidePoints[nOutsidePointCount++] = &(p[i]);
        }
    }

    if (nInsidePointCount == 0) {
        // do nothing
    } else if (nInsidePointCount == 3) {
        validTriangles.push_back(*this);
    } else if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
        Triangle newTri;

        newTri.p[0] = *insidePoints[0];
        newTri.p[1] = Vect3D::intersectPlane(plane_p, plane_n, *insidePoints[0], *outsidePoints[0]);
        newTri.p[2] = Vect3D::intersectPlane(plane_p, plane_n, *insidePoints[0], *outsidePoints[1]);
        newTri.color = color;

        validTriangles.push_back(newTri);
    } else if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
        Triangle newTri1;
        Triangle newTri2;

        newTri1.p[0] = *insidePoints[0];
        newTri1.p[1] = *insidePoints[1];
        newTri1.p[2] = Vect3D::intersectPlane(plane_p, plane_n, *insidePoints[0], *outsidePoints[0]);
        newTri1.color = color;

        newTri2.p[0] = *insidePoints[1];
        newTri2.p[1] = Vect3D::intersectPlane(plane_p, plane_n, *insidePoints[0], *outsidePoints[0]);
        newTri2.p[2] = Vect3D::intersectPlane(plane_p, plane_n, *insidePoints[1], *outsidePoints[0]);
        newTri2.color = color;

        validTriangles.push_back(newTri1);
        validTriangles.push_back(newTri2);
    }

    return validTriangles;
}

/**
 * Create a mesh from a .obj file
 *
 * @param filename the name of the .obj file
 * @return true if succeed, false otherwise
 */
bool Mesh::loadFromObjFile(const std::string filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        return false;

    std::string line;
    std::vector<Vect3D> verts;

    while (std::getline(file, line)) {
        std::stringstream stream;
        stream << line;

        char junk;

        if (line[0] == 'v') {
            Vect3D tmpV;

            stream >> junk >> tmpV.x >> tmpV.y >> tmpV.z;
            verts.push_back(tmpV);
        } else if (line[0] == 'f') {
            int indexes[3];

            stream >> junk >> indexes[0] >> indexes[1] >> indexes[2];

            Triangle tri;
            tri.p[0] = verts[indexes[0] - 1];
            tri.p[1] = verts[indexes[1] - 1];
            tri.p[2] = verts[indexes[2] - 1];
            tri.color = {255, 255, 255};
            tris.push_back(tri);
        }
    }

    return true;
}
