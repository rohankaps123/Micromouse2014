#ifndef ROBOTMODEL_H
#define ROBOTMODEL_H

//Needed for Mouse Pofile
#include "RobotModel_StepperMotor.h"

#define NORTH 	0
#define EAST 	1
#define SOUTH 	2
#define WEST	3



//Our Mouse Structure
typedef struct Mouse
{
	//Which direction are we facing?
	volatile int direction;
	
	//Robot Coordinantes
	volatile int x;
	volatile int y;
	
	//Motor Profile
	volatile StepperMotor leftMotor;
	volatile StepperMotor rightMotor;
	
	//Mouse Movements Values
	volatile float acceleration;
	volatile float deceleration;
	volatile float maxVelocity;
	volatile float velocity;
	
	//Values
	volatile int IR_CORRECT;
	//FloodFill maze;
	
} Mouse;

//Read Sensors to determine if there is a wall
int isWallFront(void);
int isWallRight(void);
int isWallLeft(void);

//Motor Controls
void moveForward(void);
void moveForwardAndStop(void);
void rotateLeft(void);
void rotateLeftWithFix(float angle);
void rotateRight(void);
void rotateRightWithFix(float angle);
void moveBackwards(void);
void moveBackwardsAndCorrect(void);
void fixAngle(float angle);




#endif