#include "RRTOptimized.h"
#include "utils.h"
#include <immintrin.h>
#include <cfloat>

#define MAX_TOLERANCE 0.001

#if defined(__x86_64__) || defined(_M_X64)
inline float rsqrt(const float f)
{
    __m128 temp = _mm_set_ss(f);
    temp = _mm_rsqrt_ss(temp);
    return _mm_cvtss_f32(temp);
}
#else
inline float rqsqrt(const float f) return 1/sqrt(f);
#endif

RRTOptimized::RRTOptimized(sf::Vector2f start, sf::Vector2f goal, const std::vector<sf::RectangleShape>& obstacles, float stepSize)
    : start(start), goal(goal), obstacles(obstacles), stepSize(stepSize), goalNode(nullptr) {
    tree.push_back(new Node(start));
}

float RRTOptimized::distance2(sf::Vector2f p1, sf::Vector2f p2) const {
    return (pow(p1.x - p2.x, 2) + pow(p2.y - p1.y, 2));
}

float RRTOptimized::distance2(float x1, float y1, float x2, float y2) const {
    return (pow(x1 - x2, 2) + pow(y2 - y1, 2));
}

Node* RRTOptimized::nearestNode(sf::Vector2f point) {
    Node* nearest = tree[0];
    float minDist = distance2(point, tree[0]->position);
    
    for (auto& node : tree) {
        float dist = distance2(point, node->position);
        if (dist < minDist) {
            minDist = dist;
            nearest = node;
        }
    }
    return nearest;
}

bool RRTOptimized::run() {
    bool stopCondition = false;

    int maxIt = 0;
    int prescedentIt = 0;
    
    while ((!stopCondition || prescedentIt > 0) && maxIt < 100000) {
        maxIt++;

        if (stopCondition) {
            prescedentIt --;
        }
        // sample starts at goal node
        sf::Vector2f sample(goal);
        
        // Find the nearest node
        Node* nearest = nearestNode(sample);

        sf::Vector2f direction = sample - nearest->position;
        direction *= rsqrt(direction.x * direction.x + direction.y * direction.y);
        
        // move towards goal
        sf::Vector2f newPoint = nearest->position + direction * stepSize;

        Line newPath(nearest->position.x, nearest->position.y, newPoint.x, newPoint.y);
        bool found = true;
        sf::Vector2f randPoint;
        sf::Vector2f collisionPoint = collision(newPath);
        if (collisionPoint.x != -1) {
            found = false;
            // search around the collision point

            for (int i = 0; i < 1000; ++i) {
                randPoint.x = static_cast<float>(randint((int) collisionPoint.x - stepSize, (int) collisionPoint.x + stepSize));
                randPoint.y = static_cast<float>(randint((int) collisionPoint.y - stepSize, (int) collisionPoint.y + stepSize));

                if (collision(nearest->position, randPoint).x != -1) {
                    newPoint = randPoint;
                    found = true;
                    break;
                }
            }
            // add no new nodes
            if (!found) 
                continue;
        }

        if (!found) std::cout << maxIt << " | broke trough\n";

        Node* newNode = new Node(newPoint, nearest);
        //std::cout << "pushing new node with " << collisionFree(nearest->position, newPoint) << std::endl;
        tree.push_back(newNode);

        // Check if the new node is close to the goal
        if (distance2(newNode->position, goal) < stepSize*stepSize) {
            goalNode = newNode;  
            
            std::cout << "final RRTOptimized It: " << maxIt << "\n";
            std::cout << "final RRTOptimized Distance: " << getPathDistance() << "\n";

            getPath();
            return true;
        }
    }
    return false;
}

sf::Vector2f RRTOptimized::collision(Line& line) const {
    float minDist = FLT_MAX;
    sf::Vector2f minPoint(-1, -1);
    for (auto& obstacle : obstacles) {
        sf::Vector2f intersec = linetorect(line, obstacle.getGlobalBounds());

        float dist = distance2(intersec.x, intersec.y, line.x1, line.x2);
        if (intersec.x != -1 && dist < minDist) {
            minDist = dist;
            minPoint = intersec;
        }
    }
    return minPoint;
}

sf::Vector2f RRTOptimized::collision(sf::Vector2f p1, sf::Vector2f p2) const {
    Line line(p1.x, p1.y, p2.x, p2.y);
    return collision(line);
}


sf::Vector2f RRTOptimized::linetoline(Line& l1, Line& l2) const {
    float den = (l1.x1 - l1.x2)*(l2.y1 - l2.y2) - (l1.y1 - l1.y2)*(l2.x1 - l2.x2);

    if (std::abs(den) < MAX_TOLERANCE)
        return sf::Vector2f(l1.x1, l1.y1);

    float t = (l1.x1 - l2.x1)*(l2.y1 - l2.y2) - (l1.y1 - l2.y1)*(l2.x1 - l2.x2);
    float u = (l1.x1 - l1.x2)*(l1.y1 - l2.y1) - (l1.y1 - l1.y2)*(l1.x1 - l2.x1);

    if (0 <= t && t <= den && 0 <= u && u <= den) {
        t /= den;
        return sf::Vector2f(l1.x1 + t*(l1.x2 - l1.x1), l1.y1 + t*(l1.y2 - l1.y1));
    }

    return sf::Vector2f(-1, -1);
}

sf::Vector2f RRTOptimized::linetorect(Line& line, sf::FloatRect rect) const {
    Line rectLines[4] = {
        Line(rect.left, rect.top, rect.left, rect.top + rect.height), // left
        Line(rect.left, rect.top, rect.left + rect.width, rect.top), //top
        Line(rect.left + rect.width, rect.top, rect.left + rect.width, rect.top + rect.height), // right
        Line(rect.left, rect.height + rect.height, rect.left + rect.width, rect.top + rect.height) // bottom
    };

    if (line.x1 >= rect.left && line.x1 <= rect.left + rect.width && line.y1 >= rect.top && line.y1 <= rect.top + rect.height) 
        return sf::Vector2f(line.x1, line.y1);

    float minDist = FLT_MAX;
    sf::Vector2f firstIn(-1, -1);
    for (Line& l : rectLines) {
        sf::Vector2f intersec = linetoline(line, l);
        if (intersec.x == -1) continue;

        float d2 = distance2(line.x1, line.y1, intersec.x, intersec.y);
        if (d2 < minDist) {
            minDist = d2;
            firstIn = intersec;
        }
    }

    return firstIn;
}

float RRTOptimized::getPathDistance() const {
    return pathLenght;
}

std::vector<sf::Vector2f> RRTOptimized::getPath() {
    pathLenght = 0;
    path = new std::vector<sf::Vector2f>();
    if (path->size() > 0) {
        return *path;
    }
    if (goalNode) {
        Node* current = goalNode;
        while (current != nullptr) {
            // std::cout << "path dist: " << pathLenght << "\n";
            if(current->parent != nullptr) pathLenght += sqrt(distance2(current->position, current->parent->position));
            path->push_back(current->position);
            current = current->parent;
        }
    }

    std::cout << "final RRTOptimized Distance: " << getPathDistance() << "\n";
    return *path;
}

void RRTOptimized::drawPath(sf::RenderWindow& window) const {
    for (size_t i = 1; i < path->size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(path->at(i-1), sf::Color::Red),
            sf::Vertex(path->at(i), sf::Color::Red)
        };
        window.draw(line, 2, sf::Lines);
    }

    sf::Vertex line[] = {
        sf::Vertex(goalNode->position, sf::Color::Red),
        sf::Vertex(goal + sf::Vector2f(3,3), sf::Color::Red)
    };
    window.draw(line, 2, sf::Lines);
}

void RRTOptimized::drawTree(sf::RenderWindow& window) const {
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
