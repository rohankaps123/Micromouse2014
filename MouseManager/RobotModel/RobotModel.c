/******************************
         RobotModel.c

 A model that controls the
 micromouse.  Includes IR and 
 stepper motor functions
 *****************************/

//AVR include
#include <avr/io.h>
#include <util/delay.h>

//Robot Profile
#include "RobotModel.h"   
#include "RobotModel_Controls.h" 
#include "RobotModel_Sensors.h"

//Debugging
#include "../USART.h"

//Create Mouse
volatile Mouse mouse;

//Return boolean value of it there is a wall infront
int isWallFront()
{
    float value = mouse.sensor[LEFT_FRONT_IR].value;
	float value2 = mouse.sensor[RIGHT_FRONT_IR].value;
	float value3 = mouse.sensor[LEFT_FRONT_IR].previousValue;
	float value4 = mouse.sensor[RIGHT_FRONT_IR].previousValue;
	
	//Average front sensors
	value = (value + value2 + value3 + value4)/4;
	
	//If there is something less then 16 cm away from sensor		
	return (value < 15);
}

//Return boolean value of if there is a wall right
int isWallRight()
{
	float value = mouse.sensor[RIGHT_IR].value; //getRightIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 14);
}

//Return boolean value of if there is a wall left
int isWallLeft()
{
	float value = mouse.sensor[LEFT_IR].value;//getLeftIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 14);
}

//Move forward, with endspeed as close to maxSpeed as possible at set acceleration
void moveForward()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(762, mouse.velocity, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
}

//Stop from current speed to 0 in the given distance/deceleration
void StopFromSpeedHalf()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(381, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);

}
//More forward half a block, with endspeed at maxSpeed
void moveForwardHalf()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(381, 0, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
}

//Stop to 0 given a start speed and decleration
void moveForwardAndStop()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(762, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

//Turn around
void moveBackwards()
{
	//Rotate Left
	setDirection(0, 1);
	
	//Move Mouse
	straight(640, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

//Use IR's to correct
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

//Move forward a number of blocks at fastest posible speed
void moveForwardBlocks(int count)
{
	//Make the robot go forward
	setDirection(0, 0);

	int mouseFastSpeed = 2500;
	
	//Adjust speed based off of set speed mode
	if(mouse.speedMode == SLOW_MODE)
		mouseFastSpeed = 2500;
	else if(mouse.speedMode == MEDIUM_MODE)
		mouseFastSpeed = 3500;
	else if(mouse.speedMode == FAST_MODE)
		mouseFastSpeed = 6000;
	else if(mouse.speedMode == SUPER_MODE)
		mouseFastSpeed = 8000;
	
	//Move mouse
	straight(762*count-0, mouse.velocity, mouseFastSpeed, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
}

//Turn around and back into wall to recorrect
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

//Rotate 90 deg left
void rotateLeft()
{
	//Rotate Left
	setDirection(0, 1);
	
	//Move Mouse
	straight(320, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

//Rotate 90 deg right
void rotateRight()
{
	//Rotate Right
	setDirection(1, 0);
		
	//Move Mouse
	straight(320, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

//Rotate 90 deg left with angle correction 
void rotateLeftWithFix(float angle)
{
	//Rotate Left
	setDirection(0, 1);
	
	int addAngle = angle*1.5;
	
	straight(320+addAngle, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration); 	
}

//Rotate 90 deg right with angle correction
void rotateRightWithFix(float angle)
{
	setDirection(1, 0);

	int addAngle = -angle*1.5;
	
	straight(320+addAngle, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration); 	
}

//Reset Encoders
void resetMotorSteps()
{
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
}

