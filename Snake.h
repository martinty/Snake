//
// Created by tysse on 21.08.2018.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <vector>

struct Position{
    int row;
    int col;
};

struct Cost{
    int F;
    int G;
    int H;
};

struct Parent{
    Position pos;
    bool active;
};

struct Tile{
    bool snake;
    bool edge;
    bool food;
    Position snakeNeighbor;
    Cost cost;
    std::vector<Position> children;
    Parent parent;
    bool path;
};

enum Direction {UP, DOWN, LEFT, RIGHT};

class Snake {
private:
    int height;
    int width;
    Direction dir;
    Position startPos;
    Position endPos;
    Position food;
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
    void moveSnakeSimulation (Direction dirSim);
    void makeNewFood();
    void setFoodPos(int row, int col);
    void addChild(int row, int col, Position child);
    void addParent(int row, int col, Parent parent);
    void setCost(int row, int col, Cost newCost);
    Cost getCost(int row, int col) const;
    Position getFoodPos() const;
    std::vector<Position> getChildren(int row, int col) const;
    Parent getParent(int row, int col) const;
    Cost costFunction(Position child);
    std::vector<Position> findChildrenAndAddChildrenAndParent();
    Direction pathFinder();
    void resetBoardAfterPathFinder();
    bool isTilePath(int row, int col) const;
};

#endif //SNAKE_SNAKE_H
