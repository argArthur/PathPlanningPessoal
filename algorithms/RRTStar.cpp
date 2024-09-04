#include "RRTStar.h"

RRTStar::RRTStar(sf::Vector2f start, sf::Vector2f goal, const std::vector<sf::RectangleShape>& obstacles, float stepSize, float radius)
    : start(start), goal(goal), obstacles(obstacles), stepSize(stepSize), radius(radius), goalNode(nullptr) {
    tree.push_back(new Node(start));
}

float RRTStar::distance(sf::Vector2f p1, sf::Vector2f p2) const {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p2.y - p1.y, 2));
}

Node* RRTStar::nearestNode(sf::Vector2f point) {
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

std::vector<Node*> RRTStar::nearNodes(sf::Vector2f point, float radius) {
    std::vector<Node*> near;
    for (auto& node : tree) {
        if (distance(point, node->position) <= radius) {
            near.push_back(node);
        }
    }
    return near;
}

bool RRTStar::collisionFree(sf::Vector2f p1, sf::Vector2f p2) const {
    sf::FloatRect lineBounds(p1, p2 - p1);
    for (auto& obstacle : obstacles) {
        if (obstacle.getGlobalBounds().intersects(lineBounds)) {
            return false;
        }
    }
    return true;
}

bool RRTStar::run() {
    while (true) {
        // Sample a random point
        sf::Vector2f randPoint(static_cast<float>(rand() % 800), static_cast<float>(rand() % 600));
        
        // Find the nearest node
        Node* nearest = nearestNode(randPoint);
        
        // Generate a new node in the direction of the sampled point
        sf::Vector2f direction = randPoint - nearest->position;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
        
        sf::Vector2f newPoint = nearest->position + direction * stepSize;
        if (collisionFree(nearest->position, newPoint)) {
            Node* newNode = new Node(newPoint, nearest, nearest->cost + distance(nearest->position, newPoint));
            tree.push_back(newNode);
            
            // Rewire the tree with new node
            std::vector<Node*> near = nearNodes(newPoint, radius);
            for (auto& nearNode : near) {
                if (collisionFree(nearNode->position, newPoint) && nearNode->cost + distance(nearNode->position, newPoint) < newNode->cost) {
                    newNode->parent = nearNode;
                    newNode->cost = nearNode->cost + distance(nearNode->position, newPoint);
                }
            }

            // Check if the new node is close to the goal
            if (distance(newNode->position, goal) < stepSize) {
                goalNode = newNode;
                return true;
            }
        }
    }
    return false;
}

std::vector<sf::Vector2f> RRTStar::getPath() const {
    std::vector<sf::Vector2f> path;
    if (goalNode) {
        Node* current = goalNode;
        while (current != nullptr) {
            path.push_back(current->position);
            current = current->parent;
        }
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void RRTStar::drawPath(sf::RenderWindow& window) const {
    std::vector<sf::Vector2f> path = getPath();
    for (size_t i = 1; i < path.size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(path[i - 1], sf::Color::Green),
            sf::Vertex(path[i], sf::Color::Green)
        };
        window.draw(line, 2, sf::Lines);
    }
}
