/*
** EPITECH PROJECT, 2020
** 3DGraphicsEngine
** File description:
** main
*/

#include <cmath>
#include <chrono>
#include <vector>

#include <fstream>
#include <sstream>

#include <algorithm>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef unsigned char uSmall; // 0 - 255

struct Vect3D {
    float x, y, z;

    void operator/=(const float &n);
};

void Vect3D::operator/=(const float &n)
{
    x /= n;
    y /= n;
    z /= n;
}

Vect3D operator+(const Vect3D &l, const float &r)
{
    Vect3D result;

    result.x = l.x + r;
    result.y = l.y + r;
    result.z = l.z + r;
    return result;
}

Vect3D operator+(const Vect3D &l, const Vect3D &r)
{
    Vect3D result;

    result.x = l.x + r.x;
    result.y = l.y + r.y;
    result.z = l.z + r.z;
    return result;
}

Vect3D operator-(const Vect3D &l, const Vect3D &r)
{
    Vect3D result;

    result.x = l.x - r.x;
    result.y = l.y - r.y;
    result.z = l.z - r.z;
    return result;
}

struct Color {
    uSmall r, g, b;
};

struct Triangle {
    Vect3D p[3];
    Color color;
};

struct Mesh {
    std::vector<Triangle> tris;

    bool loadFromObjFile(const std::string filename);
};

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

        void drawTriangle(const Triangle &) noexcept;
        void mutliplyMatrixVector(const Vect3D &i, Vect3D &o, const Matrix4x4 &m) const noexcept;

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
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * M_PI);

    _matProj.m[0][0] = fAspectRatio * fFovRad;
    _matProj.m[1][1] = fFovRad;
    _matProj.m[2][2] = fFar / (fFar - fNear);
    _matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    _matProj.m[2][3] = 1.0f;
    _matProj.m[3][3] = 0.0f;
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

    std::vector<Triangle> trianglesToRaster;

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
        triTranslated.p[0].z = triRotatedZX.p[0].z + 8.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 8.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 8.0f;

        Vect3D line1 = triTranslated.p[1] - triTranslated.p[0];
        Vect3D line2 = triTranslated.p[2] - triTranslated.p[0];
        Vect3D normal = {
            line1.y * line2.z - line1.z * line2.y,
            line1.z * line2.x - line1.x * line2.z,
            line1.x * line2.y - line1.y * line2.x
        };

        float l = sqrtf(
            normal.x * normal.x +
            normal.y * normal.y +
            normal.z * normal.z);

        normal /= l;

        if (normal.x * (triTranslated.p[0].x - _vCamera.x) +
            normal.y * (triTranslated.p[0].y - _vCamera.y) +
            normal.z * (triTranslated.p[0].z - _vCamera.z) < 0.0f
        ) {

            // Illumination
            Vect3D lightDirection = {0.0f, 0.0f, -1.0f};
            float l = sqrtf(
                lightDirection.x * lightDirection.x +
                lightDirection.y * lightDirection.y +
                lightDirection.z * lightDirection.z);

            lightDirection /= l;

            float dp =
                normal.x * lightDirection.x +
                normal.y * lightDirection.y +
                normal.z * lightDirection.z;

            triProjected.color = {
                (uSmall)(dp * 255),
                (uSmall)(dp * 255),
                (uSmall)(dp * 255)
            };

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

/**
 * Multiply a vector and a matrix and store the result in an output vector
 *
 * @param i input vector
 * @param o output vector
 * @param m input matrix
 */
void SFMLDrawer::mutliplyMatrixVector(const Vect3D &i, Vect3D &o, const Matrix4x4 &m) const noexcept
{
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];

    if (w != 0.0f) {
        o /= w;
    }
}

int main(void)
{
    SFMLDrawer drawer;

    return (0);
}
