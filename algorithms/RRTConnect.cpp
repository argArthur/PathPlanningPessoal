#include "RRTConnect.h"

RRTConnect::RRTConnect(sf::Vector2f start, sf::Vector2f goal, const std::vector<sf::RectangleShape>& obstacles, float stepSize)
    : start(start), goal(goal), obstacles(obstacles), stepSize(stepSize), connectionNodeStart(nullptr), connectionNodeGoal(nullptr) {
    treeStart.push_back(new Node(start));
    treeGoal.push_back(new Node(goal));
}

float RRTConnect::distance(sf::Vector2f p1, sf::Vector2f p2) const {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p2.y - p1.y, 2));
}

Node* RRTConnect::nearestNode(const std::vector<Node*>& tree, sf::Vector2f point) {
    Node* nearest = tree[0];
    float minDist = distance(point, tree[0]->position);
    
    for (auto& node : tree) {
        float dist = distance(point, node->position);
        if (dist < minDist) {
            minDist = dist;
            nearest = node;
        }
    }
    return nearest;
}

bool RRTConnect::collisionFree(sf::Vector2f p1, sf::Vector2f p2) const {
    sf::FloatRect lineBounds(p1, p2 - p1);
    for (auto& obstacle : obstacles) {
        if (obstacle.getGlobalBounds().intersects(lineBounds)) {
            return false;
        }
    }
    return true;
}

bool RRTConnect::connect(std::vector<Node*>& tree, Node* node) {
    while (true) {
        Node* nearest = nearestNode(tree, node->position);
        sf::Vector2f direction = node->position - nearest->position;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
        
        sf::Vector2f newPoint = nearest->position + direction * stepSize;
        if (distance(newPoint, node->position) < stepSize) {
            newPoint = node->position;
        }

        if (collisionFree(nearest->position, newPoint)) {
            Node* newNode = new Node(newPoint, nearest);
            tree.push_back(newNode);
            
            if (newPoint == node->position) {
                return true;
            }
        } else {
            return false;
        }
    }
}

bool RRTConnect::run() {
    while (true) {
        sf::Vector2f randPoint(static_cast<float>(rand() % 800), static_cast<float>(rand() % 600));
        
        Node* nearestStart = nearestNode(treeStart, randPoint);
        sf::Vector2f direction = randPoint - nearestStart->position;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
        
        sf::Vector2f newPoint = nearestStart->position + direction * stepSize;
        if (collisionFree(nearestStart->position, newPoint)) {
            Node* newNode = new Node(newPoint, nearestStart);
            treeStart.push_back(newNode);
            
            if (connect(treeGoal, newNode)) {
                connectionNodeStart = newNode;
                connectionNodeGoal = treeGoal.back();
                return true;
            }
        }

        std::swap(treeStart, treeGoal);
    }
    return false;
}

std::vector<sf::Vector2f> RRTConnect::getPath() const {
    std::vector<sf::Vector2f> path;

    if (connectionNodeStart && connectionNodeGoal) {
        Node* current = connectionNodeStart;
        while (current != nullptr) {
            path.push_back(current->position);
            current = current->parent;
        }
        std::reverse(path.begin(), path.end());

        current = connectionNodeGoal->parent;  // Skip the connecting node already added from the start tree
        while (current != nullptr) {
            path.push_back(current->position);
            current = current->parent;
        }
    }

    return path;
}

void RRTConnect::drawPath(sf::RenderWindow& window) const {
    std::vector<sf::Vector2f> path = getPath();
    for (size_t i = 1; i < path.size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(path[i - 1], sf::Color(191, 0, 255)),
            sf::Vertex(path[i], sf::Color(191, 0, 255))
        };
        window.draw(line, 2, sf::Lines);
    }
}
