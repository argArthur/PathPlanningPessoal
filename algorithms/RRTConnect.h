#ifndef RRT_CONNECT_H
#define RRT_CONNECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "utils.h"

class RRTConnect {
public:
    RRTConnect(sf::Vector2f start, sf::Vector2f goal, const std::vector<sf::RectangleShape>& obstacles, float stepSize = 10.0f);

    bool run();
    std::vector<sf::Vector2f> getPath() const;
    void drawPath(sf::RenderWindow& window) const;

private:
    Node* nearestNode(const std::vector<Node*>& tree, sf::Vector2f point);
    bool collisionFree(sf::Vector2f p1, sf::Vector2f p2) const;
    float distance(sf::Vector2f p1, sf::Vector2f p2) const;

    bool connect(std::vector<Node*>& tree, Node* node);

    std::vector<Node*> treeStart;
    std::vector<Node*> treeGoal;
    sf::Vector2f start;
    sf::Vector2f goal;
    const std::vector<sf::RectangleShape>& obstacles;
    float stepSize;
    Node* connectionNodeStart;
    Node* connectionNodeGoal;
};

#endif // RRT_CONNECT_H
