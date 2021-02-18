/*
** EPITECH PROJECT, 2021
** 3DGraphicsEngine
** File description:
** GraphicEngine3D
*/

#ifndef GRAPHICENGINE3D_HPP_
#define GRAPHICENGINE3D_HPP_

#include <list>

#include "Mesh.hpp"
#include "Vect3D.hpp"
#include "Matrix4x4.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define DISPLAY_DISTANCE 50

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

#endif /* !GRAPHICENGINE3D_HPP_ */
