//
// Created by tysse on 21.08.2018.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

struct Position{
    int row;
    int col;
};

struct Tile{
    bool snake;
    bool edge;
    bool food;
    Position snakeNeighbor;
};

enum Direction {UP, DOWN, LEFT, RIGHT};

class Snake {
private:
    int height;
    int width;
    Direction dir;
    Position startPos;
    Position endPos;
    Tile *gameBoard;
    bool alive;
public:
    Snake(int w, int h);
    ~Snake();
    bool isTileSnake(int row, int col) const;
    bool isTileFood(int row, int col) const;
    bool isTileEdge(int row, int col) const;
    void setTileSnake(int row, int col, bool active);
    void setTileFood(int row, int col, bool active);
    void setTileEdge(int row, int col, bool active);
    void setDirection(Direction input);
    Direction getDirection();
    void setSnakeNeighbor(int row, int col, Position neighbor);
    Position getSnakeNeighbor(int row, int col) const;
    bool isSnakeAlive() const;
    void initialization();
    int randomWithLimits(int min, int max) const;
    void moveSnake();
    void makeNewFood();
};

#endif //SNAKE_SNAKE_H
