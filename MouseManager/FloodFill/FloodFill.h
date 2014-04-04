#ifndef FLOODFILL_H
#define FLOODFILL_H

void initializeMaze(long cell[][16]);
void floodFill(long mazecells[16][16], char goal, int endX, int endY);
#endif