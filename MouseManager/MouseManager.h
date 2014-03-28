#ifndef MOUSEMANAGER_H
#define MOUSEMANAGER_H


void solveMaze(void);
void initializeMouse(void);
void stopMouse(void);
int isJunction(void);

void saveCurrentMaze(void);
void readSavedMaze(void);
int readMemByte(int position);
void writeMemByte(int position, int value);
int UserInterfaceIntro(void);
void FastRun(void);
#endif