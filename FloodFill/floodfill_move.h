#ifndef FLOODFILL_MOVE_H
#define	FLOODFILL_MOVE_H

int getDistance(long mazecells[16][16], int x, int y);
int getNeighborDistance(long mazecells[16][16], int x, int y, int direction);
int determineNextMove(long mazecells[16][16], int x, int y, int direction);

#endif