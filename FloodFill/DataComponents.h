#ifndef DATACOMP_H
#define DATACOMP_H

#define NORTH 		0b0000000000001000
#define EAST 		0b0000000000000100
#define SOUTH		0b0000000000000010
#define WEST		0b0000000000000001
#define EXPLORED	0b0000000000010000
#define DEP_DIRECT	0b0000000001100000
#define DISTANCE	0b0111111110000000

//Functions to get and store info from and to integer data cells

//Getting components
int getN(long input);
int getE(long input);
int getS(long input);
int getW(long input);
int getExp(long input);
int getExitDir(long input);
int getDist(long input);
int getX(long input);
int getY(long input);

//Setting components: var=variable to change, val=value to give
void setN(long *var, int val);
void setE(long *var, int val);
void setS(long *var, int val);
void setW(long *var, int val);
void setExp(long *var, int val);
void setExitDir(long *var, int val);
void setDistance(long *var, int val);
void setXY(long *var, int x, int y);
void setVarWithVal(long *var, long val, long bits);
#endif
