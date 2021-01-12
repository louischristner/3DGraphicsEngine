/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Mesh
*/

#include "../headers/Mesh.hpp"

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
            tris.push_back({
                verts[indexes[0] - 1],
                verts[indexes[1] - 1],
                verts[indexes[2] - 1]
            });
        }
    }

    return true;
}
