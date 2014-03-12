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
int getN(int input);
int getE(int input);
int getS(int input);
int getW(int input);
int getExp(int input);
int getExitDir(int input);
int getDist(int input);
//Setting components: var=variable to change, val=value to give
void setN(int *var, int val);
void setE(int *var, int val);
void setS(int *var, int val);
void setW(int *var, int val);
void setExp(int *var, int val);
void setExitDir(int *var, int val);
void setDistance(int *var, int val);
void setVarWithVal(int *var, int val, int bitOn);
#endif
