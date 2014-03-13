#ifndef FLOODFILL_MOVE_H
#define	FLOODFILL_MOVE_H

int getDistance(int x, int y);
int getNeighborDistance(int x, int y, int direction);
int determineNextMove(int x, int y);

#endif