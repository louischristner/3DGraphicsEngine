/*
** EPITECH PROJECT, 2020
** 3DGraphicsEngine
** File description:
** main
*/

#include <cmath>
#include <chrono>
#include <vector>
#include <random>

#include <algorithm>

#include "headers/GraphicEngine3D.hpp"

void handleMovements(GraphicEngine3D &engine, const float &fElapsedTime)
{
    float fYaw = engine.getFYaw();
    Vect3D vCamera = engine.getVCamera();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        vCamera.y += 8.0f * fElapsedTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        vCamera.y -= 8.0f * fElapsedTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        vCamera.x += 8.0f * fElapsedTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        vCamera.x -= 8.0f * fElapsedTime;

    Vect3D vForward = engine.getVLookDir() * (8.0f * fElapsedTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        vCamera += vForward;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        vCamera -= vForward;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        fYaw -= 2.0f * fElapsedTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        fYaw += 2.0f * fElapsedTime;

    engine.setFYaw(fYaw);
    engine.setVCamera(vCamera);
}

void generatePerlinNoise2D(const size_t &width, const size_t &height, const std::vector<float> &seed, size_t octaves, const float &bias, std::vector<float> &output)
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

                float sampleT = (1.0f - blendX) * seed[sampleY1 * width + sampleX1] + blendX * seed[sampleY1 * width + sampleX2];
                float sampleB = (1.0f - blendX) * seed[sampleY2 * width + sampleX1] + blendX * seed[sampleY2 * width + sampleX2];

                scaleAcc += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = scale / bias;
            }

            output[y * width + x] = noise / scaleAcc;
        }
    }
}

Color getColorBasedOnHeight(const Triangle &triangle, const std::vector<float> &output, const size_t &size, const float &heightMultiplier)
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

    if (averageHeight == 0.4f) {
        Triangle tmpTriangle = triangle;
        tmpTriangle.p[0].y = output[tmpTriangle.p[0].x * size + tmpTriangle.p[0].z] * heightMultiplier;
        tmpTriangle.p[1].y = output[tmpTriangle.p[1].x * size + tmpTriangle.p[1].z] * heightMultiplier;
        tmpTriangle.p[2].y = output[tmpTriangle.p[2].x * size + tmpTriangle.p[2].z] * heightMultiplier;

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

Mesh generateTerrainMesh(const size_t &size, const size_t &octaves, const float &bias)
{
    Mesh mesh;
    float heightMultiplier = 6.0f;
    std::vector<float> seed(size * size, .0f);
    std::vector<float> output(size * size, .0f);
    std::vector<float> flattened(size * size, .0f);

    std::srand(std::time(nullptr));
    for (size_t i = 0; i < size * size; i++)
        seed[i] = (float)std::rand() / (float)RAND_MAX;

    generatePerlinNoise2D(size, size, seed, octaves, bias, output);

    for (size_t i = 0; i < size * size; i++)
        flattened[i] = ((output[i] < 0.4f) ?
            0.4f : output[i]) * heightMultiplier;

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1; j++) {
            Triangle triangle1;
            triangle1.p[0] = {(float)i, flattened[i * size + (j + 1)], (float)(j + 1)};
            triangle1.p[1] = {(float)(i + 1), flattened[(i + 1) * size + j], (float)j};
            triangle1.p[2] = {(float)i, flattened[i * size + j], (float)j};
            triangle1.color = getColorBasedOnHeight(triangle1, output, size, heightMultiplier);

            Triangle triangle2;
            triangle2.p[0] = {(float)i, flattened[i * size + (j + 1)], (float)(j + 1)};
            triangle2.p[1] = {(float)(i + 1), flattened[(i + 1) * size + (j + 1)], (float)(j + 1)};
            triangle2.p[2] = {(float)(i + 1), flattened[(i + 1) * size + j], (float)j};
            triangle2.color = getColorBasedOnHeight(triangle2, output, size, heightMultiplier);

            mesh.tris.push_back(triangle1);
            mesh.tris.push_back(triangle2);
        }
    }

    return mesh;
}

int main(int ac, char **av)
{
    GraphicEngine3D engine;
    std::vector<Mesh> meshes;

    float bias = (ac >= 4) ? std::stof(av[3]) : 0.4f;
    size_t octaves = (ac >= 3) ? std::stoul(av[2]) : 7;
    size_t terrainSize = (ac >= 2) ? std::stoul(av[1]) : 200;

    meshes.push_back(
        generateTerrainMesh(terrainSize, octaves, bias));

    sf::Event event;
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Window");

    float fElapsedTime = .0f;
    std::chrono::system_clock::time_point start =
        std::chrono::system_clock::now();

    window.setFramerateLimit(30);

    while (window.isOpen()) {

        fElapsedTime = (std::chrono::system_clock::now() - start).count() / 1000000000.0f;
        start = std::chrono::system_clock::now();

        std::vector<Triangle> triangles =
            engine.projectMeshes(meshes, fElapsedTime);

        engine.displayTriangles(triangles, window);

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        handleMovements(engine, fElapsedTime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            meshes.clear();
            meshes.push_back(
                generateTerrainMesh(terrainSize, octaves, bias));
        }
    }

    return (0);
}
