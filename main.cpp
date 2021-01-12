/*
** EPITECH PROJECT, 2020
** 3DGraphicsEngine
** File description:
** main
*/

#include <cmath>
#include <chrono>
#include <vector>

#include <algorithm>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "headers/Vect3D.hpp"
#include "headers/Mesh.hpp"
#include "headers/Matrix4x4.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class IGraphicsDrawer {
    public:
        virtual ~IGraphicsDrawer() = default;

        virtual void onCreate() = 0;
        virtual void onUpdate() = 0;
};

class SFMLDrawer : public IGraphicsDrawer {
    public:
        SFMLDrawer();
        virtual ~SFMLDrawer() = default;

        void onCreate() override;
        void onUpdate() override;

    protected:
    private:
        sf::Event _event;
        sf::RenderWindow _window;
        std::chrono::_V2::system_clock::time_point _start;

        void drawTriangle(const Triangle &) noexcept;

        float _fTheta = 0;

        Mesh _meshCube;
        Matrix4x4 _matProj;

        Vect3D _vCamera;
};

SFMLDrawer::SFMLDrawer(): _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Window")
{
    this->onCreate();
    while (_window.isOpen())
        this->onUpdate();
}

/**
 * Called one time at the beginning to init the drawer
 *
 * Init the window, chrono & projection matrix
 */
void SFMLDrawer::onCreate()
{

    _window.setFramerateLimit(30);
    _start = std::chrono::system_clock::now();

    _meshCube.loadFromObjFile("spaceship.obj");

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;

    _matProj = Matrix4x4::createProjection(fFov, fAspectRatio, fNear, fFar);
}

/**
 * Called every tick to update the drawer objects
 *
 * Compute each triangle before display, then display
 */
void SFMLDrawer::onUpdate()
{
    while (_window.pollEvent(_event))
        if (_event.type == sf::Event::Closed)
            _window.close();

    _window.clear(sf::Color::Black);

    float fElapsedTime = (std::chrono::system_clock::now() - _start).count() / 1000000000.0f;
    _fTheta += 1.0f * fElapsedTime;

    _start = std::chrono::system_clock::now();

    // Rotation matrixes
    Matrix4x4 matRotX = Matrix4x4::createRotationX(_fTheta * 0.5f);
    Matrix4x4 matRotZ = Matrix4x4::createRotationZ(_fTheta);

    Matrix4x4 matTrans = Matrix4x4::createTranslation(0.0f, 0.0f, 8.0f);
    // Matrix4x4 matWorld = Matrix4x4::createIdentity();
    Matrix4x4 matWorld = (matRotZ * matRotX) * matTrans;

    std::vector<Triangle> trianglesToRaster;

    for (auto tri : _meshCube.tris) {
        Triangle triProjected;
        Triangle triTransformed;

        triTransformed.p[0] = matWorld * tri.p[0];
        triTransformed.p[1] = matWorld * tri.p[1];
        triTransformed.p[2] = matWorld * tri.p[2];

        Vect3D line1 = triTransformed.p[1] - triTransformed.p[0];
        Vect3D line2 = triTransformed.p[2] - triTransformed.p[0];
        Vect3D normal = {
            line1.y * line2.z - line1.z * line2.y,
            line1.z * line2.x - line1.x * line2.z,
            line1.x * line2.y - line1.y * line2.x
        };

        normal = normal.normalize();

        Vect3D vCameraRay = triTransformed.p[0] - _vCamera;

        if (normal.dotProduct(vCameraRay) < 0.0f) {

            // Illumination
            Vect3D lightDirection = {0.0f, 0.0f, -1.0f};
            lightDirection = lightDirection.normalize();

            float dp = normal.dotProduct(lightDirection);

            triProjected.color = {
                (uSmall)(dp * 255),
                (uSmall)(dp * 255),
                (uSmall)(dp * 255)
            };

            // Project triangle from 3D to 2D
            triProjected.p[0] = _matProj * triTransformed.p[0];
            triProjected.p[1] = _matProj * triTransformed.p[1];
            triProjected.p[2] = _matProj * triTransformed.p[2];

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

            trianglesToRaster.push_back(triProjected);
        }
    }

    // Sort triangles from back to front
    std::sort(trianglesToRaster.begin(), trianglesToRaster.end(), [](const Triangle &t1, const Triangle &t2){
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;

        return z1 > z2;
    });

    // Display triangles
    for (const auto &triProjected : trianglesToRaster) {
        this->drawTriangle(triProjected);
    }

    _window.display();
}

/**
 * Draw a triangle
 *
 * @param tri triangle to draw
 */
void SFMLDrawer::drawTriangle(const Triangle &tri) noexcept
{
    sf::ConvexShape triangle(3);

    triangle.setOutlineColor(sf::Color::White);
    triangle.setFillColor(sf::Color(tri.color.r, tri.color.g, tri.color.b));

    triangle.setPoint(0, sf::Vector2f(tri.p[0].x, tri.p[0].y));
    triangle.setPoint(1, sf::Vector2f(tri.p[1].x, tri.p[1].y));
    triangle.setPoint(2, sf::Vector2f(tri.p[2].x, tri.p[2].y));

    _window.draw(triangle);
}

int main(void)
{
    SFMLDrawer drawer;

    return (0);
}
