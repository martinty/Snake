//
// Created by tysse on 21.08.2018.
//

#include <iostream>
#include <random>
#include <cmath>
#include <vector>
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
        gameBoard[i].parent.active = false;
        gameBoard[i].path = false;
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


void Snake::moveSnakeSimulation(Direction dirSim) {
    Position oldStartPos = startPos;
    Position startPosSim = startPos;
    switch (dirSim){
        case UP:
            startPosSim.row -= 1;
            break;
        case DOWN:
            startPosSim.row += 1;
            break;
        case LEFT:
            startPosSim.col -= 1;
            break;
        case RIGHT:
            startPosSim.col += 1;
            break;
    }
    setSnakeNeighbor(oldStartPos.row, oldStartPos.col, startPosSim);
    setTileSnake(endPos.row, endPos.col, false);
    endPos = getSnakeNeighbor(endPos.row, endPos.col);
    setTileSnake(startPosSim.row, startPosSim.col, true);
}

void Snake::makeNewFood() {
    int foodRow = randomWithLimits(2, height-3);
    int foodCol = randomWithLimits(2, width-3);
    while(isTileSnake(foodRow, foodCol)){
        foodRow = randomWithLimits(2, height-3);
        foodCol = randomWithLimits(2, width-3);
    }
    setTileFood(foodRow, foodCol, true);
    setFoodPos(foodRow, foodCol);
}

void Snake::setFoodPos(int row, int col) {
    food.row = row;
    food.col = col;
}

void Snake::setCost(int row, int col, Cost newCost) {
    gameBoard[row*width+col].cost = newCost;
}

Cost Snake::getCost(int row, int col) const {
    return gameBoard[row*width+col].cost;
}


Cost Snake::costFunction(Position child) {
    Cost currentCost = getCost(startPos.row, startPos.col);
    Cost newCost;
    newCost.H = abs(child.row-getFoodPos().row) + abs(child.col-getFoodPos().col);
    if(isTileSnake(child.row, child.col)){
        newCost.G = currentCost.G + width*height*2;
    }
    else{
        newCost.G = currentCost.G + 1;
    }
    newCost.F = newCost.H + newCost.G;
    return newCost;
}

void Snake::addChild(int row, int col, Position child) {
    gameBoard[row*width+col].children.push_back(child);
}

void Snake::addParent(int row, int col, Parent parent) {
    gameBoard[row*width+col].parent = parent;
}

Position Snake::getFoodPos() const {
    return food;
}

std::vector<Position> Snake::getChildren(int row, int col) const {
    return gameBoard[row*width+col].children;
}

Parent Snake::getParent(int row, int col) const {
    return gameBoard[row*width+col].parent;
}

std::vector<Position> Snake::findChildrenAndAddChildrenAndParent() {
    vector<Position> newChildren;
    Position newChild;
    Cost newCost;
    Parent parent;
    parent.pos = startPos;
    parent.active = true;
    for (int i = -1; i < 2; i+=2) {
        if(!isTileEdge(startPos.row+i, startPos.col) && !isTileSnake(startPos.row+i, startPos.col)){
            if(!getParent(startPos.row+i, startPos.col).active){
                newChild.row = startPos.row+i;
                newChild.col = startPos.col;
                addChild(startPos.row, startPos.col, newChild);
                addParent(newChild.row, newChild.col, parent);
                newCost = costFunction(newChild);
                setCost(newChild.row, newChild.col, newCost);
                newChildren.push_back(newChild);
            }
        }
        if(!isTileEdge(startPos.row, startPos.col+i) && !isTileSnake(startPos.row, startPos.col+i)){
            if(!getParent(startPos.row, startPos.col+i).active) {
                newChild.row = startPos.row;
                newChild.col = startPos.col + i;
                addChild(startPos.row, startPos.col, newChild);
                addParent(newChild.row, newChild.col, parent);
                newCost = costFunction(newChild);
                setCost(newChild.row, newChild.col, newCost);
                newChildren.push_back(newChild);
            }
        }
    }
    return newChildren;
}


Direction Snake::pathFinder() {
    resetBoardAfterPathFinder();
    Position snake = startPos;
    std::vector<Position> openList;
    std::vector<Position> closedList;
    Cost startCost;
    startCost.H = abs(startPos.row - food.row) + abs(startPos.col - food.col);
    startCost.F = startCost.H;
    startCost.G = 0;
    setCost(startPos.row, startPos.col, startCost);
    openList.push_back(startPos);

    while(!openList.empty()){
        int bestF = getCost(openList.begin()->row, openList.begin()->col).F;
        Position bestNode = *openList.begin();
        int i = 0;
        int index = 0;
        for (auto it = std::begin(openList); it!=std::end(openList); ++it){
            if (bestF > getCost(it->row, it->col).F){
                bestF = getCost(it->row, it->col).F;
                bestNode = *it;
                index = i;
            }
            else{
                i++;
            }
        }
        startPos = bestNode;
        if(startPos.row == food.row && startPos.col == food.col){
            break;
        }
        openList.erase(openList.begin()+index);
        closedList.push_back(startPos);
        std::vector<Position> newChildren = findChildrenAndAddChildrenAndParent();
        for (auto child = std::begin(newChildren); child!=std::end(newChildren); ++child){
            openList.push_back(*child);
        }
    }

    Parent simulation = getParent(getFoodPos().row, getFoodPos().col);

    if(!simulation.active){
        return LEFT;
    }
    else{
        Position path =  simulation.pos;
        std::vector<Position> bestPath;
        startPos = snake;
        if(path.row == startPos.row && path.col == startPos.col){
            bestPath.push_back(food);
            gameBoard[food.row*width + food.col].path = true;
        }
        else{
            while (!(path.row == startPos.row && path.col == startPos.col)){
                bestPath.push_back(path);
                gameBoard[path.row*width + path.col].path = true;
                path = getParent(path.row, path.col).pos;
            }
        }
        Position move = bestPath.back();
        if(move.row == startPos.row){
            if(startPos.col < move.col){
                return RIGHT;
            }
            else {
                return LEFT;
            }
        }
        else{
            if(startPos.row < move.row){
                return DOWN;
            }
            else {
                return UP;
            }
        }
    }
}

void Snake::resetBoardAfterPathFinder() {
    Cost resetCost;
    Parent resetParent;
    resetCost.F = 0;
    resetCost.G = 0;
    resetCost.H = 0;
    resetParent.active = false;
    for (int row = 0; row < width; ++row) {
        for (int col = 0; col < height; ++col) {
            gameBoard[row*width+col].children.clear();
            gameBoard[row*width+col].parent = resetParent;
            gameBoard[row*width+col].cost = resetCost;
            gameBoard[row*width+col].path = false;
        }
    }
}

bool Snake::isTilePath(int row, int col) const {
    return gameBoard[row*width+col].path;
}