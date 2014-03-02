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
	
	//FloodFill maze;
	
} Mouse;

//Read Sensors to determine if there is a wall
int isWallForward(void);
int isWallRight(void);
int isWallLeft(void);

//Sensor Functions
int getIRSensorValue(volatile uint8_t *port, uint8_t pin, int analogChannel);
void setupADC(void);
uint16_t ReadADC(uint8_t ch);

#endif