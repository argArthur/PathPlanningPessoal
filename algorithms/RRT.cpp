#include "RRT.h"

RRT::RRT(sf::Vector2f start, sf::Vector2f goal, const std::vector<sf::RectangleShape>& obstacles, float stepSize)
    : start(start), goal(goal), obstacles(obstacles), stepSize(stepSize), goalNode(nullptr) {
    tree.push_back(new Node(start));
}

float RRT::distance(sf::Vector2f p1, sf::Vector2f p2) const {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p2.y - p1.y, 2));
}

Node* RRT::nearestNode(sf::Vector2f point) {
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

bool RRT::collisionFree(sf::Vector2f p1, sf::Vector2f p2) const {
    sf::FloatRect lineBounds(p1, p2 - p1);
    for (auto& obstacle : obstacles) {
        if (obstacle.getGlobalBounds().intersects(lineBounds)) {
            return false;
        }
    }
    return true;
}

bool RRT::run() {
    bool stopCondition = false;

    int maxIt = 0;
    int prescedentIt = 0;
    
    while ((!stopCondition || prescedentIt > 0) && maxIt < 100000) {
        maxIt++;

        if (stopCondition) {
            prescedentIt --;
        }
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
            Node* newNode = new Node(newPoint, nearest);
            tree.push_back(newNode);
            
            // Check if the new node is close to the goal
            if (distance(newNode->position, goal) < stepSize) {
                goalNode = newNode;  
                
                std::cout << "final RRT It: " << maxIt << "\n";
                std::cout << "final RRT Distance: " << getPathDistance() << "\n";

                getPath();
                return true;
            }
        }
    }
    return false;
}

float RRT::getPathDistance() const {
    return pathLenght;
}

std::vector<sf::Vector2f> RRT::getPath() {
    pathLenght = 0;
    path = new std::vector<sf::Vector2f>();
    if (path->size() > 0) {
        return *path;
    }
    if (goalNode) {
        Node* current = goalNode;
        while (current != nullptr) {
            // std::cout << "path dist: " << pathLenght << "\n";
            if(current->parent != nullptr) pathLenght += distance(current->position, current->parent->position);
            path->push_back(current->position);
            current = current->parent;
        }
    }

    std::cout << "final RRT Distance: " << getPathDistance() << "\n";
    return *path;
}

void RRT::drawPath(sf::RenderWindow& window) const {
    for (size_t i = 1; i < path->size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(path->at(i-1), sf::Color::Yellow),
            sf::Vertex(path->at(i), sf::Color::Yellow)
        };
        window.draw(line, 2, sf::Lines);
    }

    sf::Vertex line[] = {
        sf::Vertex(goalNode->position, sf::Color::Yellow),
        sf::Vertex(goal + sf::Vector2f(3,3), sf::Color::Yellow)
    };
    window.draw(line, 2, sf::Lines);
}

void RRT::drawTree(sf::RenderWindow& window) const {
    for (const auto& node : tree) {
        if (node->parent) {
            sf::Vertex line[] = {
                sf::Vertex(node->position, sf::Color(100,100,100,80)),
                sf::Vertex(node->parent->position, sf::Color(100,100,100,80))
            };
            window.draw(line, 2, sf::Lines);
        }
    }
}
