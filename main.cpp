/*
** EPITECH PROJECT, 2020
** 3DGraphicsEngine
** File description:
** main
*/

#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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

sf::ConvexShape createLine(sf::Vector2f p1, sf::Vector2f p2)
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

int main(void)
{
    /**

    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(800, 600), "My Window");

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
