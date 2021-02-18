/*
** EPITECH PROJECT, 2020
** 3DGraphicsEngine
** File description:
** main
*/

#include <list>
#include <cmath>
#include <chrono>
#include <vector>
#include <random>

#include <algorithm>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "headers/Vect3D.hpp"
#include "headers/Mesh.hpp"
#include "headers/Matrix4x4.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DISPLAY_DISTANCE 50

void drawTriangle(const Triangle &tri, sf::RenderWindow &window)
{
    sf::ConvexShape triangle(3);

    triangle.setOutlineColor(sf::Color::White);
    triangle.setFillColor(sf::Color(tri.color.r, tri.color.g, tri.color.b));

    triangle.setPoint(0, sf::Vector2f(tri.p[0].x, tri.p[0].y));
    triangle.setPoint(1, sf::Vector2f(tri.p[1].x, tri.p[1].y));
    triangle.setPoint(2, sf::Vector2f(tri.p[2].x, tri.p[2].y));

    window.draw(triangle);
}

class GraphicEngine3D
{
    public:
        GraphicEngine3D();
        virtual ~GraphicEngine3D() = default;

        float getFTheta(void) const;
        void setFTheta(const float &v);

        float getFYaw(void) const;
        void setFYaw(const float &v);

        Vect3D getVCamera(void) const;
        void setVCamera(const Vect3D &v);

        Vect3D getVLookDir(void) const;
        void setVLookDir(const Vect3D &v);

        std::vector<Triangle> projectMeshes(const std::vector<Mesh> &meshes, const float &fElapsedTime);
        void displayTriangles(const std::vector<Triangle> &triangles, sf::RenderWindow &window);

    private:
        float _fTheta = .0f;
        float _fYaw = .0f;

        Matrix4x4 _matProj;

        Vect3D _vCamera;
        Vect3D _vLookDir;
};

GraphicEngine3D::GraphicEngine3D()
{
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;

    _matProj = Matrix4x4::createProjection(fFov, fAspectRatio, fNear, fFar);
}

float GraphicEngine3D::getFTheta(void) const
{
    return _fTheta;
}

void GraphicEngine3D::setFTheta(const float &v)
{
    _fTheta = v;
}

float GraphicEngine3D::getFYaw(void) const
{
    return _fYaw;
}

void GraphicEngine3D::setFYaw(const float &v)
{
    _fYaw = v;
}

Vect3D GraphicEngine3D::getVCamera(void) const
{
    return _vCamera;
}

void GraphicEngine3D::setVCamera(const Vect3D &v)
{
    _vCamera = v;
}

Vect3D GraphicEngine3D::getVLookDir(void) const
{
    return _vLookDir;
}

void GraphicEngine3D::setVLookDir(const Vect3D &v)
{
    _vLookDir = v;
}

std::vector<Triangle> GraphicEngine3D::projectMeshes(const std::vector<Mesh> &meshes, const float &fElapsedTime)
{
    std::vector<Triangle> trianglesToDisplay;
    // _fTheta += 1.0f * fElapsedTime;

    // Rotation matrixes
    Matrix4x4 matRotX = Matrix4x4::createRotationX(_fTheta);
    Matrix4x4 matRotZ = Matrix4x4::createRotationZ(_fTheta * 0.5f);

    Matrix4x4 matTrans = Matrix4x4::createTranslation(0.0f, 0.0f, 5.0f);
    Matrix4x4 matWorld = (matRotZ * matRotX) * matTrans;

    Vect3D vUp = {0, 1, 0};
    Vect3D vTarget = {0, 0, 1};
    _vLookDir = Matrix4x4::createRotationY(_fYaw) * vTarget;
    vTarget = _vCamera + _vLookDir;

    Matrix4x4 matCamera = Matrix4x4::createPointAt(_vCamera, vTarget, vUp);
    Matrix4x4 matView = matCamera.quickInverse();

    for (auto mesh : meshes) {
        for (auto tri : mesh.tris) {

            float distanceWithCamera0 = std::sqrt((tri.p[0].x - _vCamera.x) * (tri.p[0].x - _vCamera.x) + (tri.p[0].y - _vCamera.y) * (tri.p[0].y - _vCamera.y) + (tri.p[0].z - _vCamera.z) * (tri.p[0].z - _vCamera.z));
            float distanceWithCamera1 = std::sqrt((tri.p[1].x - _vCamera.x) * (tri.p[1].x - _vCamera.x) + (tri.p[1].y - _vCamera.y) * (tri.p[1].y - _vCamera.y) + (tri.p[1].z - _vCamera.z) * (tri.p[1].z - _vCamera.z));
            float distanceWithCamera2 = std::sqrt((tri.p[2].x - _vCamera.x) * (tri.p[2].x - _vCamera.x) + (tri.p[2].y - _vCamera.y) * (tri.p[2].y - _vCamera.y) + (tri.p[2].z - _vCamera.z) * (tri.p[2].z - _vCamera.z));

            float averageDistanceWithCamera = (distanceWithCamera0 + distanceWithCamera1 + distanceWithCamera2) / 3;

            if (averageDistanceWithCamera > DISPLAY_DISTANCE)
                continue;

            Triangle triProjected;
            Triangle triTransformed;
            Triangle triViewed;

            triTransformed.p[0] = matWorld * tri.p[0];
            triTransformed.p[1] = matWorld * tri.p[1];
            triTransformed.p[2] = matWorld * tri.p[2];

            Vect3D line1 = triTransformed.p[1] - triTransformed.p[0];
            Vect3D line2 = triTransformed.p[2] - triTransformed.p[0];
            Vect3D normal = line1.crossProduct(line2);

            normal = normal.normalize();

            Vect3D vCameraRay = triTransformed.p[0] - _vCamera;

            if (normal.dotProduct(vCameraRay) < 0.0f) {

                // Illumination
                Vect3D lightDirection = {0.0f, 1.0f, -1.0f};
                lightDirection = lightDirection.normalize();

                float dp = std::max(0.1f, lightDirection.dotProduct(normal));

                triTransformed.color = {
                    (uSmall)(dp * tri.color.r),
                    (uSmall)(dp * tri.color.g),
                    (uSmall)(dp * tri.color.b)
                };

                // Convert world space to view space
                triViewed.p[0] = matView * triTransformed.p[0];
                triViewed.p[1] = matView * triTransformed.p[1];
                triViewed.p[2] = matView * triTransformed.p[2];
                triViewed.color = triTransformed.color;

                std::vector<Triangle> clippedTriangles = triViewed.clipAgainstPlane({.0f, .0f, .1f}, {.0f, .0f, .1f});

                for (const auto &clippedTriangle : clippedTriangles) {

                    // Project triangle from 3D to 2D
                    triProjected.p[0] = _matProj * clippedTriangle.p[0];
                    triProjected.p[1] = _matProj * clippedTriangle.p[1];
                    triProjected.p[2] = _matProj * clippedTriangle.p[2];
                    triProjected.color = clippedTriangle.color;

                    triProjected.p[0].x *= -1.0f;
                    triProjected.p[1].x *= -1.0f;
                    triProjected.p[2].x *= -1.0f;
                    triProjected.p[0].y *= -1.0f;
                    triProjected.p[1].y *= -1.0f;
                    triProjected.p[2].y *= -1.0f;

                    // Scale into view
                    Vect3D vOffsetView = {1.0f, 1.0f, .0f};

                    triProjected.p[0] += vOffsetView;
                    triProjected.p[1] += vOffsetView;
                    triProjected.p[2] += vOffsetView;

                    triProjected.p[0].x *= 0.5f * (float)WINDOW_WIDTH;
                    triProjected.p[0].y *= 0.5f * (float)WINDOW_HEIGHT;
                    triProjected.p[1].x *= 0.5f * (float)WINDOW_WIDTH;
                    triProjected.p[1].y *= 0.5f * (float)WINDOW_HEIGHT;
                    triProjected.p[2].x *= 0.5f * (float)WINDOW_WIDTH;
                    triProjected.p[2].y *= 0.5f * (float)WINDOW_HEIGHT;

                    trianglesToDisplay.push_back(triProjected);
                }
            }
        }
    }

    std::sort(trianglesToDisplay.begin(), trianglesToDisplay.end(), [](const Triangle &t1, const Triangle &t2){
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;

        return z1 > z2;
    });

    return trianglesToDisplay;
}

void GraphicEngine3D::displayTriangles(const std::vector<Triangle> &triangles, sf::RenderWindow &window)
{
    const Vect3D planeStarts[4] = {
        {0.0f, 0.0f, 0.0f},
        {0.0f, (float)WINDOW_HEIGHT - 1, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {(float)WINDOW_WIDTH - 1, 0.0f, 0.0f}
    };

    const Vect3D planeEnds[4] = {
        {0.0f, 1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f}
    };

    window.clear(sf::Color::Black);

    for (const auto &triProjected : triangles) {

        std::list<Triangle> listTriangles;
        std::vector<Triangle> validTriangles;

        listTriangles.push_back(triProjected);

        size_t nNewTriangles = 1;

        for (int p = 0; p < 4; p++) {
            while (nNewTriangles > 0) {
                Triangle triToTest = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles -= 1;

                validTriangles = triToTest.clipAgainstPlane(planeStarts[p], planeEnds[p]);

                for (size_t w = 0; w < validTriangles.size(); w++)
                    listTriangles.push_back(validTriangles[w]);
            }

            nNewTriangles = listTriangles.size();
        }

        for (const auto &triToDisplay : listTriangles) {
            drawTriangle(triToDisplay, window);
        }
    }

    window.display();
}

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
