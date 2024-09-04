#ifndef RRT_STAR_H
#define RRT_STAR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "utils.h"

class RRTStar {
public:
    RRTStar(sf::Vector2f start, sf::Vector2f goal, const std::vector<sf::RectangleShape>& obstacles, float stepSize = 10.0f, float radius = 50.0f);

    bool run();
    std::vector<sf::Vector2f> getPath() const;
    void drawPath(sf::RenderWindow& window) const;

private:
    Node* nearestNode(sf::Vector2f point);
    std::vector<Node*> nearNodes(sf::Vector2f point, float radius);
    bool collisionFree(sf::Vector2f p1, sf::Vector2f p2) const;
    float distance(sf::Vector2f p1, sf::Vector2f p2) const;

    std::vector<Node*> tree;
    sf::Vector2f start;
    sf::Vector2f goal;
    const std::vector<sf::RectangleShape>& obstacles;
    float stepSize;
    float radius;
    Node* goalNode;
};

#endif // RRT_STAR_H
