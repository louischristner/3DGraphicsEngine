/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Chunk
*/

#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <vector>

#include "Mesh.hpp"
#include "Vect3D.hpp"

class Chunk
{
    public:
        Chunk(const int &x, const int &y);
        virtual ~Chunk() = default;

        void generateTerrain(const size_t &octaves, const float &bias);
        Color getColorBasedOnHeight(const Triangle &triangle, const std::vector<float> &output, const float &heightMultiplier) const;

        Mesh getMesh(void) const;

        int getX(void) const;
        int getY(void) const;
        size_t getSize(void) const;

    protected:
    private:
        int _x;
        int _y;

        Mesh _mesh;

        std::vector<float> _seed;

        const size_t _size = 60;
};

#endif /* !CHUNK_HPP_ */
