#include "../RobotModel/RobotModel.h"

#define FORWARD 0
#define REVERSE 1
#define LEFT 2
#define RIGHT 3

#define USELESS 1024

extern volatile Mouse mouse;

int determineNextMove(long mazecells[16][16], int x, int y) {
    int bestDistance = getDistance(x, y);
    int bestDirection;

    if ((bestDistance > getNeighborDistance(x, y, mouse.direction)) &&
            (isWallFront() == 0)) {
        bestDirection = mouse.direction;
        bestDistance = getNeighborDistance(x, y, bestDirection);
    }

    if ((bestDistance > getNeighborDistance(x, y, NORTH)) &&
            (getWall(mouse.direction, NORTH) == 0)) {
        bestDirection = NORTH;
        bestDistance = getNeighborDistance(x, y, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(x, y, EAST)) &&
            (getWall(mouse.direction, EAST) == 0)) {
        bestDirection = EAST;
        bestDistance = getNeighborDistance(x, y, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(x, y, WEST)) &&
            (getWall(mouse.direction, WEST) == 0)) {
        bestDirection = WEST;
        bestDistance = getNeighborDistance(x, y, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(x, y, SOUTH)) &&
            (getWall(mouse.direction, SOUTH) == 0)) {
        bestDirection = SOUTH;
        bestDistance = getNeighborDistance(x, y, bestDirection);
    }
    
    return bestDirection;
}

int getNeighborDistance(int x, int y, int direction) {
    int neighborX;
    int neighborY;
    if ((direction == NORTH) && (y != 1)) {
        neighborX = x;
        neighborY = y - 1;
    } else if ((direction == SOUTH) && (y != 16)) {
        neighbor = x;
        neighborY = y + 1;
    } else if ((direction == EAST) && (x != 16)) {
        neighborX = x + 1;
        neighborY = y;
    } else if ((direction == WEST) && (x != 1)) {
        neighborX = x - 1;
        neighborY = y;
    } else {
        return USELESS;
    }

    return getDistance(neighborX, neighborY);
}

int getDistance(int x, int y) {
    return getDist(cells[x - 1][y - 1]);
}
