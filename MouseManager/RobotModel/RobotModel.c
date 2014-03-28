/******************************
         RobotModel.c

 A model that controls the
 micromouse.  Includes IR and 
 stepper motor functions
 *****************************/

#include <avr/io.h>
#include <util/delay.h>

#include "RobotModel.h"   
#include "RobotModel_Controls.h" 
#include "RobotModel_Sensors.h"

#include "../USART.h"

//Create Mouse
volatile Mouse mouse;

int isWallFront()
{
    float value = mouse.sensor[LEFT_FRONT_IR].value;
	float value2 = mouse.sensor[RIGHT_FRONT_IR].value;
	float value3 = mouse.sensor[LEFT_FRONT_IR].previousValue;
	float value4 = mouse.sensor[RIGHT_FRONT_IR].previousValue;
	
	//Average front sensors
	value = (value + value2 + value3 + value4)/4;
	
	//If there is something less then 16 cm away from sensor
	//return (value < 10);		
	return (value < 15);
}

int isWallRight()
{
	float value = mouse.sensor[RIGHT_IR].value; //getRightIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 14);
}

int isWallLeft()
{
	float value = mouse.sensor[LEFT_IR].value;//getLeftIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 14);
}

void moveForward()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(762, mouse.velocity, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
}

void StopFromSpeedHalf()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(381, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);

}
void moveForwardHalf()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(381, 0, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
}

void moveForwardAndStop()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(762, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void moveBackwards()
{
	//Rotate Left
	setDirection(0, 1);
	
	//Move Mouse
	straight(640, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void moveFix(int length)
{
	if(length > 0)
	{
		setDirection(1, 1);
	}
	else
	{
		setDirection(0, 0);
		length = -length;
	}

	setDirection(0, 0);
	straight(length*42, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void moveForwardBlocks(int count)
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(762*count-50, mouse.velocity, 4000, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
}

void moveBackwardsAndCorrect()
{		
	//Rotate Left 180
	setDirection(0, 1);
	straight(640, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
	
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	//Go Backwards
	setDirection(1, 1);	
	straight(250, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
	
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	setDirection(0, 0);
	straight(130, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void rotateLeft()
{
	//Rotate Left
	setDirection(0, 1);
	
	//Move Mouse
	straight(320, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void rotateRight()
{
	//Rotate Right
	setDirection(1, 0);
		
	//Move Mouse
	straight(320, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void rotateLeftWithFix(float angle)
{
	//Rotate Left
	setDirection(0, 1);
	
	int addAngle = angle*1.5;
	
	straight(320+addAngle, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration); 	
}

void rotateRightWithFix(float angle)
{
	setDirection(1, 0);

	int addAngle = -angle*1.5;
	
	straight(320+addAngle, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration); 	
}

void fixAngle(float angle)
{
	if(angle > 0)
	{
		setDirection(0, 1);
	}
	else
	{
		setDirection(1, 0);
		angle = -angle;
	}
	
	straight((int)angle*3, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration); 
}

void resetMotorSteps()
{
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
}

