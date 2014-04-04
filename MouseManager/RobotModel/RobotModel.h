#ifndef ROBOTMODEL_H
#define ROBOTMODEL_H

//Needed for Mouse Pofile
#include "RobotModel_StepperMotor.h"
#include "RobotModel_Sensors.h"

#define WHEEL_CIRCUM 		18.8495 //Cm
#define STEPS_PER_ROTATION	800 
#define MOUSE_WIDTH			10 //Cm
#define MOUSE_WIDTH_HALF	MOUSE_WIDTH / 2

#define TURN_CONST (2 * 3.141529 * STEPS_PER_ROTATION) / (360 * WHEEL_CIRCUM) //Multiply by Radius (cm) and Degrees to get arcLength

//Directions
#define dNORTH 	0
#define dWEST 	1
#define dSOUTH 	2
#define dEAST	4

//Sensor Array Numbers
#define LEFT_IR			0
#define RIGHT_IR		1
#define LEFT_FRONT_IR	2
#define RIGHT_FRONT_IR	3

#define SLOW_MODE		0 //2500 step/sec
#define MEDIUM_MODE		1 //3500 step/sec
#define FAST_MODE		2 //6000 step/sec
#define SUPER_MODE		3 //8000 step/sec

typedef struct Vector
{
	int x;
	int y;
} Vector;

//Our Mouse Structure
typedef struct Mouse
{
	//Which direction are we facing?
	volatile Vector direction;
	
	//Robot Coordinantes
	volatile int x;
	volatile int y;
	
	//Motor Profile
	volatile StepperMotor leftMotor;
	volatile StepperMotor rightMotor;
	
	//Mouse Sensors
	volatile Sensor sensor[4];
	volatile int currentChannel;//For ADC conversion
	volatile int speedMode;
	
	//Mouse Movements Values
	volatile float acceleration;
	volatile float deceleration;
	volatile float maxVelocity;
	volatile float velocity;
	
	//Values
	volatile int IR_CORRECT;
	volatile int IR_CORRECT_LEFT;
	volatile int IR_CORRECT_RIGHT;
	
	//Longitude corrections
	volatile int IR_LONG_CHECK_LEFT;
	volatile int IR_LONG_CHECK_RIGHT;
	volatile double IR_LONG_OFF_DISTANCE_LEFT;
	volatile double IR_LONG_OFF_DISTANCE_RIGHT;
	
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
void StopFromSpeedHalf(void);
void moveForwardHalf(void);
void resetMotorStepCount(void);
void moveForwardBlocks(int count);

#endif