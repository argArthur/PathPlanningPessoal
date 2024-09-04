#pragma once

#include <SFML/Graphics.hpp>

int randint(int min, int max);

struct Node {
    sf::Vector2f position;
    Node* parent;
    float cost;

    Node(sf::Vector2f pos, Node* par = nullptr, float cost = 0.0f) 
        : position(pos), parent(par), cost(cost) {}
};

