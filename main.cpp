/*
** EPITECH PROJECT, 2020
** 3DGraphicsEngine
** File description:
** main
*/

#include <cmath>
#include <chrono>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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

        void drawTriangle(Triangle) noexcept;
        sf::ConvexShape createLine(sf::Vector2f p1, sf::Vector2f p2) const noexcept;
        void mutliplyMatrixVector(Vect3D &i, Vect3D &o, Matrix4x4 &m) const noexcept;

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

void SFMLDrawer::onCreate()
{

    _window.setFramerateLimit(30);
    _start = std::chrono::system_clock::now();

    _meshCube.tris = {

        // SOUTH
        { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

        // EAST
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

        // NORTH
        { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

        // WEST
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

        // TOP
        { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

        // BOTTOM
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

    };

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * M_PI);

    _matProj.m[0][0] = fAspectRatio * fFovRad;
    _matProj.m[1][1] = fFovRad;
    _matProj.m[2][2] = fFar / (fFar - fNear);
    _matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    _matProj.m[2][3] = 1.0f;
    _matProj.m[3][3] = 0.0f;
}

void SFMLDrawer::onUpdate()
{
    while (_window.pollEvent(_event))
        if (_event.type == sf::Event::Closed)
            _window.close();

    _window.clear(sf::Color::Black);

    Matrix4x4 matRotX;
    Matrix4x4 matRotZ;

    float fElapsedTime = (std::chrono::system_clock::now() - _start).count() / 1000000000.0f;
    _fTheta += 1.0f * fElapsedTime;

    _start = std::chrono::system_clock::now();

    // Rotation X
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(_fTheta * 0.5f);
    matRotX.m[1][2] = sinf(_fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(_fTheta * 0.5f);
    matRotX.m[2][2] = cosf(_fTheta * 0.5f);
    matRotX.m[3][3] = 1;

    // Rotation Y
    matRotZ.m[0][0] = cosf(_fTheta);
    matRotZ.m[0][1] = sinf(_fTheta);
    matRotZ.m[1][0] = -sinf(_fTheta);
    matRotZ.m[1][1] = cosf(_fTheta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    for (auto tri : _meshCube.tris) {
        Triangle triRotatedZ;
        Triangle triRotatedZX;
        Triangle triTranslated;
        Triangle triProjected;

        mutliplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
        mutliplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
        mutliplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

        mutliplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
        mutliplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
        mutliplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

        Vect3D line1;
        Vect3D line2;
        Vect3D normal;

        line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
        line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
        line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

        line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
        line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
        line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

        normal.x = line1.y * line2.z - line1.z * line2.y;
        normal.y = line1.z * line2.x - line1.x * line2.z;
        normal.z = line1.x * line2.y - line1.y * line2.x;

        float l = sqrtf(
            normal.x * normal.x +
            normal.y * normal.y +
            normal.z * normal.z);

        normal.x /= l;
        normal.y /= l;
        normal.z /= l;

        if (normal.x * (triTranslated.p[0].x - _vCamera.x) +
            normal.y * (triTranslated.p[0].y - _vCamera.y) +
            normal.z * (triTranslated.p[0].z - _vCamera.z) < 0.0f
        ) {

            // Project triangle from 3D to 2D
            mutliplyMatrixVector(triTranslated.p[0], triProjected.p[0], _matProj);
            mutliplyMatrixVector(triTranslated.p[1], triProjected.p[1], _matProj);
            mutliplyMatrixVector(triTranslated.p[2], triProjected.p[2], _matProj);

            // Scale into view
            triProjected.p[0].x += 1.0f;
            triProjected.p[0].y += 1.0f;
            triProjected.p[1].x += 1.0f;
            triProjected.p[1].y += 1.0f;
            triProjected.p[2].x += 1.0f;
            triProjected.p[2].y += 1.0f;

            triProjected.p[0].x *= 0.5f * (float)WINDOW_WIDTH;
            triProjected.p[0].y *= 0.5f * (float)WINDOW_HEIGHT;
            triProjected.p[1].x *= 0.5f * (float)WINDOW_WIDTH;
            triProjected.p[1].y *= 0.5f * (float)WINDOW_HEIGHT;
            triProjected.p[2].x *= 0.5f * (float)WINDOW_WIDTH;
            triProjected.p[2].y *= 0.5f * (float)WINDOW_HEIGHT;

            this->drawTriangle(triProjected);

        }
    }

    _window.display();
}

void SFMLDrawer::drawTriangle(Triangle tri) noexcept
{
    _window.draw(this->createLine(sf::Vector2f(tri.p[0].x, tri.p[0].y), sf::Vector2f(tri.p[1].x, tri.p[1].y)));
    _window.draw(this->createLine(sf::Vector2f(tri.p[1].x, tri.p[1].y), sf::Vector2f(tri.p[2].x, tri.p[2].y)));
    _window.draw(this->createLine(sf::Vector2f(tri.p[2].x, tri.p[2].y), sf::Vector2f(tri.p[0].x, tri.p[0].y)));
}

sf::ConvexShape SFMLDrawer::createLine(sf::Vector2f p1, sf::Vector2f p2) const noexcept
{
    sf::ConvexShape convexShape(4);

    convexShape.setOutlineThickness(1);
    convexShape.setOutlineColor(sf::Color::White);

    convexShape.setPoint(0, p1);
    convexShape.setPoint(1, p2);
    convexShape.setPoint(2, p2);
    convexShape.setPoint(3, p1);

    return convexShape;
}

void SFMLDrawer::mutliplyMatrixVector(Vect3D &i, Vect3D &o, Matrix4x4 &m) const noexcept
{
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];

    if (w != 0.0f) {
        o.x /= w;
        o.y /= w;
        o.z /= w;
    }
}

int main(void)
{
    SFMLDrawer drawer;

    /**

    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Window");

    while (window.isOpen()) {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::Black);
        window.draw(createLine(sf::Vector2f(100, 100), sf::Vector2f(200, 200)));
        window.display();
    }

    */

    return (0);
}
