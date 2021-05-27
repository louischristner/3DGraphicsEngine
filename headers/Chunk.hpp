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
        void generatePerlinNoise2D(const size_t &width, const size_t &height, const size_t &octaves, const float &bias);
        void generateSquare(const int &i, const int &j, const std::vector<float> &flattened, const float &heightMultiplier, Triangle &triangle1, Triangle &triangle2);
        Color getColorBasedOnHeight(const Triangle &triangle, const float &heightMultiplier) const;

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
        std::vector<float> _noise;

        const size_t _size = 60;
};

#endif /* !CHUNK_HPP_ */
