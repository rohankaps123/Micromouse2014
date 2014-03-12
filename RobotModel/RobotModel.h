#ifndef ROBOTMODEL_H
#define ROBOTMODEL_H

//Needed for Mouse Pofile
#include "RobotModel_StepperMotor.h"

#define WHEEL_CIRCUM 		18.8495 //Cm
#define STEPS_PER_ROTATION	800 
#define MOUSE_WIDTH			10 //Cm
#define MOUSE_WIDTH_HALF	MOUSE_WIDTH / 2

#define TURN_CONST (2 * 3.141529 * STEPS_PER_ROTATION) / (360 * WHEEL_CIRCUM) //Multiply by Radius (cm) and Degrees to get arcLength

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
	volatile int IR_CORRECT_LEFT;
	volatile int IR_CORRECT_RIGHT;
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