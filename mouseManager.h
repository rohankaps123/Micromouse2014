#ifndef MOUSEMANAGER_H
#define MOUSEMANAGER_H


void solveMaze(void);
void initializeMouse(void);
void stopMouse(void);
void updateWalls(void);
void whereToGo(void);
int wallExists(long data, int dirx, int diry);

#endif