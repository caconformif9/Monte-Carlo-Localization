#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
class Environment {
public:
    struct Obstacle {
        float width = 0;
        float height = 0;
        sf::Vector2f position;

        sf::RectangleShape square;
        Obstacle() : width(rand() % 50 + 20 + 1),height(rand() % 50 + 20 + 1),position(rand() % 750 + 50 +1,rand() % 550 + 50 + 1), square(sf::Vector2f(width, height)) {
            square.setPosition(position);
            square.setFillColor(sf::Color::Red);
            square.setOutlineThickness(2.f);
            square.setOutlineColor(sf::Color::Black);
        }
    };

    std::vector<Obstacle> obstacles;

public:
    Environment() {
        srand(static_cast<unsigned>(time(nullptr)));
        int n = 7 + rand() % 15;
        for (int i = 0; i < n; i++) {
            obstacles.emplace_back();   
        }
    }

    void draw(sf::RenderWindow& win) {
        for (auto& it : obstacles) {
            win.draw(it.square);
        }
    }
};


#endif

