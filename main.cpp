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

#include <iostream>

#include "headers/Chunk.hpp"
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

void putChunksInMeshes(const size_t &octaves, const float &bias, std::vector<Chunk> &chunks, std::vector<Mesh> &meshes)
{
    meshes.clear();
    for (auto &chunk : chunks) {
        chunk.generateTerrain(octaves, bias);
        meshes.push_back(chunk.getMesh());
    }
}

int main(int ac, char **av)
{
    GraphicEngine3D engine;
    std::vector<Mesh> meshes;

    float bias = (ac >= 3) ? std::stof(av[2]) : 0.8f;
    size_t octaves = (ac >= 2) ? std::stoul(av[1]) : 5;

    std::vector<Chunk> chunks;

    std::srand(std::time(nullptr));
    for (size_t y = 0; y < 2; y++)
        for (size_t x = 0; x < 2; x++)
            chunks.push_back(Chunk(-60 + (60 * x), -60 + (60 * y)));

    putChunksInMeshes(octaves, bias, chunks, meshes);

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
            putChunksInMeshes(octaves, bias, chunks, meshes);
        }
    }

    return (0);
}
