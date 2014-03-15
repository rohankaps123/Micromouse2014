#include "floodfill_move.h"
#include "DataComponents.h"

#define USELESS 1024

int determineNextMove(long mazecells[16][16], int x, int y, int direction) {
    int bestDistance = getDistance(mazecells, x, y);
    int bestDirection = direction;
    
    int wall = USELESS;
    switch (direction) {
        case NORTH:
            wall = getN(mazecells[x][y]);
            break;
        case EAST:
            wall = getE(mazecells[x][y]);
            break;
        case WEST:
            wall = getW(mazecells[x][y]);
            break;
        case SOUTH:
            wall = getS(mazecells[x][y]);
            break;
    }
    if ((bestDistance > getNeighborDistance(mazecells, x, y, direction)) && (wall == 0)) {
        bestDirection = direction;
        bestDistance = getNeighborDistance(mazecells, x, y, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(mazecells, x, y, NORTH)) && (getN(mazecells[x][y]) == 0)) {
        bestDirection = NORTH;
        bestDistance = getNeighborDistance(mazecells, x, y, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(mazecells, x, y, EAST)) && (getE(mazecells[x][y]) == 0)) {
        bestDirection = EAST;
        bestDistance = getNeighborDistance(mazecells, x, y, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(mazecells, x, y, WEST)) && (getW(mazecells[x][y]) == 0)) {
        bestDirection = WEST;
        bestDistance = getNeighborDistance(mazecells, x, y, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(mazecells, x, y, SOUTH)) && (getS(mazecells[x][y]) == 0)) {
        bestDirection = SOUTH;
        bestDistance = getNeighborDistance(mazecells, x, y, bestDirection);
    }
    
    return bestDirection;
}

//Get the distance of a neighboring cell 
int getNeighborDistance(long mazecells[16][16], int x, int y, int direction) {
    int neighborX;
    int neighborY;
    if ((direction == NORTH) && (y != 1)) {
        neighborX = x;
        neighborY = y - 1;
    } else if ((direction == SOUTH) && (y != 16)) {
        neighborX = x;
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

    return getDistance(mazecells, neighborX, neighborY);
}

int getDistance(long mazecells[16][16], int x, int y) {
    return getDist(mazecells[x - 1][y - 1]);
}
