/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** GraphicEngine3D
*/

#include "../headers/GraphicEngine3D.hpp"

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
