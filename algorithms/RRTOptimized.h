#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "utils.h"

class RRTOptimized {
public:
    RRTOptimized(sf::Vector2f start, sf::Vector2f goal, const std::vector<sf::RectangleShape>& obstacles, float stepSize = 10.0f);

    bool run();
    float getPathDistance() const;
    std::vector<sf::Vector2f> getPath();
    void drawPath(sf::RenderWindow& window) const;
    void drawTree(sf::RenderWindow& window) const;
private:
    Node* nearestNode(sf::Vector2f point);
    bool collisionFree(sf::Vector2f p1, sf::Vector2f p2) const;
    float distance2(sf::Vector2f p1, sf::Vector2f p2) const;

    std::vector<Node*> tree;
    std::vector<sf::Vector2f>* path;
    sf::Vector2f start;
    sf::Vector2f goal;
    const std::vector<sf::RectangleShape>& obstacles;
    float stepSize;
    float pathLenght;
    Node* goalNode;
};

