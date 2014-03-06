#include "RobotModel/RobotModel.h"

#define FORWARD 0
#define REVERSE 1
#define LEFT 2
#define RIGHT 3

#define USELESS 1024

int determineNextMove() {
    int bestDistance = getDistance(here);
    int bestDirection;

    if ((bestDistance > getNeighborDistance(here, robotLocation.getDirection())) &&
            (robotLocation.isWallFront() == false)) {
        bestDirection = robotLocation.getDirection();
        bestDistance = getNeighborDistance(here, bestDirection);
    }

    if ((bestDistance > getNeighborDistance(here, Direction.North)) &&
            (maze.getWall(here, Direction.North).isSet() == false)) {
        bestDirection = Direction.North;
        bestDistance = getNeighborDistance(here, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(here, Direction.East)) &&
            (maze.getWall(here, Direction.East).isSet() == false)) {
        bestDirection = Direction.East;
        bestDistance = getNeighborDistance(here, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(here, Direction.West)) &&
            (maze.getWall(here, Direction.West).isSet() == false)) {
        bestDirection = Direction.West;
        bestDistance = getNeighborDistance(here, bestDirection);
    }
    if ((bestDistance > getNeighborDistance(here, Direction.South)) &&
            (maze.getWall(here, Direction.South).isSet() == false)) {
        bestDirection = Direction.South;
        bestDistance = getNeighborDistance(here, bestDirection);
    }

    if (bestDirection == null) {
        floodfill();
        return getBestDirection();
    } else {
        return bestDirection;
    }
}

int getNeighborDistance(int x, int y, int direction) {
    int neighbor;
    if ((direction == NORTH) && (mouse.y != 1)) {
        neighbor = cells[mouse.x][mouse.y - 1];
    } else if ((direction == SOUTH) && (mouse.y != 16)) {
        neighbor = cells[mouse.x][mouse.y + 1];
    } else if ((direction == EAST) && (mouse.x != 16)) {
        neighbor = cells[mouse.x + 1][mouse.y];
    } else if ((direction == WEST) && (x != 1)) {
        neighbor = cells[mouse.x - 1][mouse.y];
    } else {
        return USELESS;
    }

    return getDistance(neighbor);
}

int getDistance(int x, int y) {
    return cells[mouse.x - 1][mouse.y - 1];
}