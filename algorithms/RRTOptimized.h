#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "utils.h"

class Line {
    public:
    Line(float x1, float y1, float x2, float y2): x1(x1), y1(y1), x2(x2), y2(y2) {};
    float x1, y1, x2, y2;
};

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
    sf::Vector2f linetoline(Line& l1, Line&l2) const;
    sf::Vector2f linetorect(Line& line, sf::FloatRect rect) const;
    sf::Vector2f collision(Line& line) const;
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

