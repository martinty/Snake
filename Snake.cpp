//
// Created by tysse on 21.08.2018.
//

#include <iostream>
#include <random>
using namespace std;
#include "Snake.h"

Snake::Snake(int w, int h) {
    width = w;
    height = h;
    gameBoard = new Tile[width*height];
    for (int i = 0; i < width*height; ++i) {
        gameBoard[i].edge = false;
        gameBoard[i].snake = false;
        gameBoard[i].food = false;
    }
    initialization();
}

Snake::~Snake() {
    delete[] gameBoard;
    gameBoard = nullptr;
}

bool Snake::isTileSnake(int row, int col) const {
    return gameBoard[row*width + col].snake;
}

bool Snake::isTileFood(int row, int col) const {
    return gameBoard[row*width + col].food;
}

bool Snake::isTileEdge(int row, int col) const {
    return gameBoard[row*width + col].edge;
}

void Snake::setTileSnake(int row, int col, bool active) {
    gameBoard[row*width + col].snake = active;
}

void Snake::setTileFood(int row, int col, bool active) {
    gameBoard[row*width + col].food = active;
}

void Snake::setTileEdge(int row, int col, bool active) {
    gameBoard[row*width + col].edge = active;
}

void Snake::setDirection(Direction input) {
    dir = input;
}

Direction Snake::getDirection() {
    return dir;
}

void Snake::setSnakeNeighbor(int row, int col, Position neighbor) {
    gameBoard[row*width + col].snakeNeighbor = neighbor;
}

Position Snake::getSnakeNeighbor(int row, int col) const {
    return gameBoard[row*width + col].snakeNeighbor;
}

bool Snake::isSnakeAlive() const {
    return alive;
}

void Snake::initialization() {
    for (int row = 0; row < height ; ++row) {
        setTileEdge(row,0, true);
        setTileEdge(row, width-1, true);
    }
    for (int col = 0; col < width; ++col) {
        setTileEdge(0, col, true);
        setTileEdge(height-1, col, true);
    }
    int startRow = width/2;
    int startCol = height/2;
    setTileSnake(startRow, startCol, true);
    for (int i = 1; i < 4; ++i) {
        Position neighbor;
        setTileSnake(startRow, startCol - i, true);
        neighbor.row = startRow;
        neighbor.col = startCol - i + 1;
        setSnakeNeighbor(startRow, startCol - i, neighbor);
    }
    dir = RIGHT;
    startPos.row = startRow;
    startPos.col = startCol;
    endPos.row = startRow;
    endPos.col = startCol-3;
    makeNewFood();
    alive = true;
}

int Snake::randomWithLimits(int min, int max) const {
    return rand() % (max+1-min) + min ;
}

void Snake::moveSnake() {
    Position oldStartPos = startPos;
    switch (dir){
        case UP:
            startPos.row -= 1;
            break;
        case DOWN:
            startPos.row += 1;
            break;
        case LEFT:
            startPos.col -= 1;
            break;
        case RIGHT:
            startPos.col += 1;
            break;
    }
    setSnakeNeighbor(oldStartPos.row, oldStartPos.col, startPos);
    if(isTileFood(startPos.row, startPos.col)){
        setTileFood(startPos.row, startPos.col, false);
        makeNewFood();
    }
    else if(isTileEdge(startPos.row, startPos.col) || isTileSnake(startPos.row, startPos.col)){
        alive = false;
    }
    else{
        setTileSnake(endPos.row, endPos.col, false);
        Position newEnd = getSnakeNeighbor(endPos.row, endPos.col);
        endPos = newEnd;
    }
    setTileSnake(startPos.row, startPos.col, true);
}

void Snake::makeNewFood() {
    int foodRow = randomWithLimits(1, height-2);
    int foodCol = randomWithLimits(1, width-2);
    while(isTileSnake(foodRow, foodCol)){
        foodRow = randomWithLimits(1, height-2);
        foodCol = randomWithLimits(1, width-2);
    }
    setTileFood(foodRow, foodCol, true);
}