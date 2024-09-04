#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>

//Include here the headers of the algorithms
#include "algorithms/RRT.h"
#include "algorithms/RRTConnect.h"
#include "algorithms/RRTStar.h"
#include "algorithms/RRTOptimized.h"

sf::RectangleShape header(sf::Vector2f(0, 0));

bool drawingDots[2] = {false, false};
sf::VertexArray currentLine(sf::Lines, 2);

bool dotsPlaced[2] = {false, false};
sf::CircleShape startPos(5);
sf::VertexArray startLine(sf::Lines, 2);
sf::CircleShape goalPos(5);
sf::VertexArray goalLine(sf::Lines, 2);

sf::Text startLabel;
sf::Text goalLabel;

bool creatingObstacle = false;
std::vector<sf::RectangleShape> obstacles;
sf::RectangleShape currentSquare;

sf::Font font;
sf::RectangleShape button(sf::Vector2f(100, 30));
sf::Text buttonText;

sf::Text RRTText, RRTSText, RRTCText, RRTOText;



RRT* rrtAlgorithm = nullptr;
RRTStar* rrtSAlgorithm = nullptr;
RRTConnect* rrtCAlgorithm = nullptr;
RRTOptimized* rrtOAlgorithm = nullptr;

float calculateAngle(sf::Vector2f p1, sf::Vector2f p2) {
    return atan2(p1.y - p2.y, p1.x - p2.x);
}

void assignPos(sf::CircleShape& pos, sf::Vector2i &mousePos) {
    pos.setPosition(mousePos.x - pos.getRadius(), mousePos.y - pos.getRadius());
}

void assignLabel(sf::Text& label, const std::string& text, sf::Vector2i& mousePos, const sf::Font& font) {
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(14);
    label.setFillColor(sf::Color::White);
    label.setPosition(mousePos.x + 10, mousePos.y + 10); 
}

void createObstacle(std::vector<sf::RectangleShape>& squares, sf::Vector2i mousePos) {
    sf::RectangleShape square(sf::Vector2f(20, 20)); // Size of the square
    square.setFillColor(sf::Color::Blue);
    square.setPosition(mousePos.x - square.getSize().x / 2, mousePos.y - square.getSize().y / 2);
    squares.push_back(square);
}

bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePos) {
    return button.getGlobalBounds().contains(mousePos.x, mousePos.y);
}

bool isMouseOverSquare(const sf::RectangleShape& square, const sf::Vector2i& mousePos) {
    return square.getGlobalBounds().contains(mousePos.x, mousePos.y);
}

bool isMouseOverHeader(const sf::RectangleShape& header, const sf::Vector2i& mousePos) {
    return header.getGlobalBounds().contains(mousePos.x, mousePos.y);
}

void updateLine(sf::VertexArray& line, sf::CircleShape& pos, const sf::Vector2i& mousePos) {
    line[0].position = sf::Vector2f(pos.getPosition().x + pos.getRadius(), pos.getPosition().y + pos.getRadius());
    line[1].position = sf::Vector2f(mousePos.x, mousePos.y);
    line[0].color = sf::Color::Yellow;
    line[1].color = sf::Color::Yellow;
}

void headerDraw(sf::RenderWindow& window) {
    window.draw(header);

    window.draw(button);
    window.draw(buttonText);
}

void windowDraw(sf::RenderWindow& window) {
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f);

    if (creatingObstacle) {
        // Set only border color
        currentSquare.setOutlineThickness(1);
        currentSquare.setOutlineColor(sf::Color::White);
        window.draw(currentSquare);
    }

    for (auto& square : obstacles) {
        window.draw(square);
    }

    if (dotsPlaced[0]) {
        window.draw(startPos);
        window.draw(startLabel);
        window.draw(startLine);
    }

    if (dotsPlaced[1]) {
        window.draw(goalPos);
        window.draw(goalLabel);
        window.draw(goalLine);
    }

    if (drawingDots[0] || drawingDots[1]) {
        window.draw(currentLine);
    }

    headerDraw(window);
}


////////////////////////////////////////////////////
//
// Add the algorithm.run() function here
//
////////////////////////////////////////////////////
void runAlgorithms(sf::Vector2f startPos, sf::Vector2f goalPos, const std::vector<sf::RectangleShape>& obstacles) {
    rrtAlgorithm = new RRT(startPos, goalPos, obstacles, 20);
    rrtAlgorithm->run();

    rrtSAlgorithm = new RRTStar(startPos, goalPos, obstacles);
    rrtSAlgorithm->run();

    rrtCAlgorithm = new RRTConnect(startPos, goalPos, obstacles);    
    rrtCAlgorithm->run();

    rrtOAlgorithm = new RRTOptimized(startPos, goalPos, obstacles, 20);
    rrtOAlgorithm->run();
}


////////////////////////////////////////////////////
//
// To do not have memory leaks, destroy the algorithms
//
////////////////////////////////////////////////////
void destroyAlgorithms() {
    if (rrtAlgorithm) delete rrtAlgorithm;
    if (rrtSAlgorithm) delete rrtSAlgorithm;
    if (rrtCAlgorithm) delete rrtCAlgorithm;
    if (rrtOAlgorithm) delete rrtOAlgorithm;

    rrtAlgorithm = nullptr;
    rrtSAlgorithm = nullptr;
    rrtCAlgorithm = nullptr;
    rrtOAlgorithm = nullptr;
}

////////////////////////////////////////////////////
//
// To print the algorithms, call the drawTree and drawPath 
// functions
//
////////////////////////////////////////////////////
void printAlgorithms(sf::RenderWindow &window) {
    if (rrtAlgorithm) {
        rrtAlgorithm->drawTree(window);
        rrtAlgorithm->drawPath(window);
    }

    if (rrtSAlgorithm) {
        rrtSAlgorithm->drawPath(window);
    }

    if (rrtCAlgorithm) {
        rrtCAlgorithm->drawPath(window);
    }

    if (rrtOAlgorithm) {
        rrtOAlgorithm->drawPath(window);
    }
}

int main() {
    // Create a window with the given dimensions and title
    sf::RenderWindow window(sf::VideoMode(800, 600), "Path Planning");

    header.setPosition(0, 0);
    header.setSize(sf::Vector2f(window.getSize().x, 40));
    header.setFillColor(sf::Color(40, 40, 40, 150));

    button.setFillColor(sf::Color(0,0,200));
    button.setPosition(5, 5);
    
    buttonText.setFont(font);
    buttonText.setString("Play");
    buttonText.setCharacterSize(16);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(button.getPosition().x + button.getSize().x / 2.0f, button.getPosition().y + button.getSize().y / 2.0f);
    
    if (!font.loadFromFile("./fonts/JetBrainsMonoNLNerdFont-ExtraLightItalic.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    bool gameStarted = false;

    startPos.setFillColor(sf::Color::Red);
    goalPos.setFillColor(sf::Color::Green);

    sf::Vector2i initialMousePos;

    // Main loop that continues until the window is closed
    while (window.isOpen()) {
        // Create an event object to handle events
        sf::Event event;
        // Process events
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse click event
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                
                if (event.mouseButton.button == sf::Mouse::Left) {
                    //Handle header clicks
                    if (isMouseOverButton(button, mousePosition)) {
                        if (dotsPlaced[0] && dotsPlaced[1] && !gameStarted) {
                            button.setFillColor(sf::Color(0,150,170)); // Change color on click
                            buttonText.setString("Stop");
                            gameStarted = true;

                            button.setFillColor(sf::Color(0,120,200)); // Change color on click
                            buttonText.setString("Play");
                            gameStarted = false;

                            runAlgorithms(startPos.getPosition(), goalPos.getPosition(), obstacles);
                        } else if (gameStarted) {
                            button.setFillColor(sf::Color(0,120,200));
                            buttonText.setString("Play");
                            gameStarted = false;

                            destroyAlgorithms();
                        }
                    }

                    if (isMouseOverHeader(header, mousePosition)) break;
                    if (creatingObstacle) break;
                    if (gameStarted) break;

                    // Handle obstacle removal clicks
                    bool obstacleClicked = false;
                    for (auto it = obstacles.begin(); it != obstacles.end(); ++it) {
                        if (isMouseOverSquare(*it, mousePosition)) {
                            obstacleClicked = true;
                            obstacles.erase(it);
                            break;
                        }
                    }
                    if (obstacleClicked) break;

                    // Handle start dot placement
                    if (!drawingDots[0]) {
                        drawingDots[0] = true;
                        assignPos(startPos, mousePosition);
                        assignLabel(startLabel, "Start", mousePosition, font);
                        dotsPlaced[0] = true;
                        updateLine(currentLine, startPos, mousePosition);
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (isMouseOverHeader(header, mousePosition)) break;
                    if (creatingObstacle) break;
                    if (gameStarted) break;

                    // Handle goal dot placement
                    if (!drawingDots[1]) {
                        drawingDots[1] = true;
                        assignPos(goalPos, mousePosition);
                        assignLabel(goalLabel, "goal", mousePosition, font);
                        dotsPlaced[1] = true;
                        updateLine(currentLine, goalPos, mousePosition);
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Middle && !gameStarted && !isMouseOverHeader(header, mousePosition)) {
                    creatingObstacle = true;

                    initialMousePos = sf::Mouse::getPosition(window);
                    currentSquare.setSize(sf::Vector2f(0, 0));
                    currentSquare.setFillColor(sf::Color::Blue);
                }
            }

            // Handle mouse drag event
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (creatingObstacle) {
                    sf::Vector2f size = sf::Vector2f(mousePosition.x - initialMousePos.x, mousePosition.y - initialMousePos.y);
                    currentSquare.setSize(size);
                    currentSquare.setPosition(initialMousePos.x, initialMousePos.y);
                }
                if (drawingDots[0]) {
                    updateLine(currentLine, startPos, mousePosition);
                } else if (drawingDots[1]) {
                    updateLine(currentLine, goalPos, mousePosition);
                }
            }

            // Handle mouse release event
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (creatingObstacle) {
                        creatingObstacle = false;
                        currentSquare.setOutlineThickness(0);
                        obstacles.push_back(currentSquare);
                    }
                    if (drawingDots[0]) {
                        drawingDots[0] = false;
                        startLine[0] = currentLine[0];
                        startLine[1] = currentLine[1];
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (drawingDots[1]) {
                        drawingDots[1] = false;
                        goalLine[0] = currentLine[0];
                        goalLine[1] = currentLine[1];
                    }
                }
            }
        }

        window.clear();

        windowDraw(window);        

        printAlgorithms(window);

        // Update the window
        window.display();
    }

    if (rrtAlgorithm) delete rrtAlgorithm;

    return 0;
}
