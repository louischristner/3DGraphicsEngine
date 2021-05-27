/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** Chunk
*/

#include "../headers/Chunk.hpp"

Chunk::Chunk(const int &x, const int &y):
    _x(x), _y(y)
{
    _seed = std::vector<float>(_size * _size, .0f);
    _noise = std::vector<float>(_size * _size, .0f);
}

void Chunk::generatePerlinNoise2D(
    const size_t &width,
    const size_t &height,
    const size_t &octaves,
    const float &bias
)
{
    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            float noise = .0f;
            float scale = 1.0f;
            float scaleAcc = .0f;

            for (size_t o = 0; o < octaves; o++) {
                size_t pitch = width >> o;
                size_t sampleX1 = (x / pitch) * pitch;
                size_t sampleY1 = (y / pitch) * pitch;

                size_t sampleX2 = (sampleX1 + pitch) % width;
                size_t sampleY2 = (sampleY1 + pitch) % width;

                float blendX = (float)(x - sampleX1) / (float)pitch;
                float blendY = (float)(y - sampleY1) / (float)pitch;

                float sampleT = (1.0f - blendX) * _seed[sampleY1 * width + sampleX1] + blendX * _seed[sampleY1 * width + sampleX2];
                float sampleB = (1.0f - blendX) * _seed[sampleY2 * width + sampleX1] + blendX * _seed[sampleY2 * width + sampleX2];

                scaleAcc += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = scale / bias;
            }

            _noise[y * width + x] = noise / scaleAcc;
        }
    }
}

Color Chunk::getColorBasedOnHeight(const Triangle &triangle, const float &heightMultiplier) const
{
    size_t index = 0;
    float averageHeight = triangle.getAverageHeight() / heightMultiplier;
    float heights[] = {0.3f, 0.4f, 0.45f, 0.55f, 0.6f, 0.7f, 0.9f, 1.0f};
    Color colors[] = {
        {20, 40, 170},
        {30, 150, 230},
        {250, 220, 25},
        {110, 200, 80},
        {0, 135, 0},
        {130, 100, 50},
        {90, 70, 40},
        {255, 255, 255}
    };

    auto getOutputIndexFromVect = [this](const Vect3D &vect)
    {
        return (vect.x - this->getX()) * this->getSize() + (vect.z - this->getY());
    };

    if (averageHeight == 0.4f) {
        Triangle tmpTriangle = triangle;

        tmpTriangle.p[0].y = _noise[getOutputIndexFromVect(triangle.p[0])] * heightMultiplier;
        tmpTriangle.p[1].y = _noise[getOutputIndexFromVect(triangle.p[1])] * heightMultiplier;
        tmpTriangle.p[2].y = _noise[getOutputIndexFromVect(triangle.p[2])] * heightMultiplier;

        averageHeight = tmpTriangle.getAverageHeight() / heightMultiplier;

        for (; index < sizeof(heights) / sizeof(float); index++)
            if (averageHeight < heights[index])
                break;
    } else {
        for (; index < sizeof(heights) / sizeof(float); index++)
            if (averageHeight < heights[index])
                break;
    }

    return colors[index];
}

void Chunk::generateTerrain(const size_t &octaves, const float &bias)
{
    size_t heightMultiplier = 6.0f;
    std::vector<float> flattened(_size * _size, .0f);

    _mesh.tris.clear();

    for (size_t i = 0; i < _size * _size; i++)
        _seed[i] = (float)std::rand() / (float)RAND_MAX;

    generatePerlinNoise2D(_size, _size, octaves, bias);

    for (size_t i = 0; i < _size * _size; i++)
        flattened[i] = ((_noise[i] < 0.4f) ?
            0.4f : _noise[i]) * heightMultiplier;

    for (int i = 0; i < _size - 1; i++) {
        for (int j = 0; j < _size - 1; j++) {
            Triangle triangle1, triangle2;
            generateSquare(i, j, flattened, heightMultiplier, triangle1, triangle2);
            _mesh.tris.push_back(triangle1);
            _mesh.tris.push_back(triangle2);
        }
    }
}

void Chunk::generateSquare(
    const int &i,
    const int &j,
    const std::vector<float> &flattened,
    const float &heightMultiplier,
    Triangle &triangle1,
    Triangle &triangle2
)
{
    // if coords out of flattened vector
    if (flattened.size() <= (i + 1) * _size + (j + 1))
        return;

    triangle1.p[0] = {(float)(_x + i), flattened[i * _size + (j + 1)], (float)(_y + j + 1)};
    triangle1.p[1] = {(float)(_x + i + 1), flattened[(i + 1) * _size + j], (float)(_y + j)};
    triangle1.p[2] = {(float)(_x + i), flattened[i * _size + j], (float)(_y + j)};
    triangle1.color = getColorBasedOnHeight(triangle1, heightMultiplier);

    triangle2.p[0] = {(float)(_x + i), flattened[i * _size + (j + 1)], (float)(_y + j + 1)};
    triangle2.p[1] = {(float)(_x + i + 1), flattened[(i + 1) * _size + (j + 1)], (float)(_y + j + 1)};
    triangle2.p[2] = {(float)(_x + i + 1), flattened[(i + 1) * _size + j], (float)(_y + j)};
    triangle2.color = getColorBasedOnHeight(triangle2, heightMultiplier);
}

Mesh Chunk::getMesh(void) const
{
    return _mesh;
}

int Chunk::getX(void) const
{
    return _x;
}

int Chunk::getY(void) const
{
    return _y;
}

size_t Chunk::getSize(void) const
{
    return _size;
}
