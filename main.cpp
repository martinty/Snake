
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
using namespace std;
#include "Snake.h"

const sf::Color TILE_COLOR = sf::Color::White;
const sf::Color SNAKE_COLOR = sf::Color::Green;
const sf::Color FOOD_COLOR = sf::Color::Red;
const sf::Color EDGE_COLOR = sf::Color::Black;
const sf::Color PATH_COLOR = sf::Color::Blue;
const int TILE_SIZE = 32;
const int HEIGHT = 30;
const int WIDTH = 30;
const int SPEED = 1;

int main() {
    srand(time(nullptr));
    auto* game = new Snake(HEIGHT,WIDTH);
    sf::RenderWindow window(sf::VideoMode(HEIGHT*TILE_SIZE, WIDTH*TILE_SIZE), "Snake", sf::Style::Close);
    window.setFramerateLimit(60);
    clock_t time = clock();
    while (window.isOpen()){
        if(clock() - time > SPEED && game->isSnakeAlive()){
            game->setDirection(game->pathFinder());
            game->moveSnake();
            time = clock();
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    Direction dir = game->getDirection();
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                        case sf::Keyboard::Q:
                            window.close();
                            break;
                        case sf::Keyboard::A:
                            if(dir != RIGHT && game->isSnakeAlive()){
                                game->setDirection(LEFT);
                                game->moveSnake();
                                time = clock();
                            }
                            break;
                        case sf::Keyboard::D:
                            if(dir != LEFT && game->isSnakeAlive()){
                                game->setDirection(RIGHT);
                                game->moveSnake();
                                time = clock();
                            }
                            break;
                        case sf::Keyboard::W:
                            if(dir != DOWN && game->isSnakeAlive()){
                                game->setDirection(UP);
                                game->moveSnake();
                                time = clock();
                            }
                            break;
                        case sf::Keyboard::S:
                            if(dir != UP && game->isSnakeAlive()){
                                game->setDirection(DOWN);
                                game->moveSnake();
                                time = clock();
                            }
                            break;
                        case sf::Keyboard::Space:
                            if(!game->isSnakeAlive()){
                                delete game;
                                game = new Snake(WIDTH, HEIGHT);
                                time = clock();
                            }
                            break;
                    }
            }
        }
        window.clear();
        for(int row = 0; row < HEIGHT; ++row) {
            for(int col = 0; col < WIDTH; ++col) {
                const int tile_x = col * TILE_SIZE, tile_y = row * TILE_SIZE;
                sf::RectangleShape tile;
                if(game->isTileEdge(row, col)){
                    tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    tile.setPosition(tile_x, tile_y);
                    tile.setFillColor(EDGE_COLOR);
                }
                else if(game->isTileSnake(row, col)){
                    tile.setSize(sf::Vector2f(TILE_SIZE-2, TILE_SIZE-2));
                    tile.setPosition(tile_x + 1, tile_y + 1);
                    tile.setFillColor(SNAKE_COLOR);
                }
                else if(game->isTileFood(row, col)){
                    tile.setSize(sf::Vector2f(TILE_SIZE-2, TILE_SIZE-2));
                    tile.setPosition(tile_x + 1, tile_y + 1);
                    tile.setFillColor(FOOD_COLOR);
                }
                else{
                    tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    tile.setPosition(tile_x, tile_y);
                    tile.setFillColor(TILE_COLOR);
                }
                window.draw(tile);

                if (game->isTilePath(row, col)){
                    tile.setSize(sf::Vector2f(TILE_SIZE/2, TILE_SIZE/2));
                    tile.setPosition(tile_x + TILE_SIZE/4, tile_y + TILE_SIZE/4);
                    tile.setFillColor(PATH_COLOR);
                    window.draw(tile);
                }
            }
        }
        window.display();
    }
    return 0;
}

