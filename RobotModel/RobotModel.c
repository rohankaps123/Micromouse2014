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
    float value = getFrontIR();
	
	//If there is something less then 16 cm away from sensor
	return (value < 16);		
}

int isWallRight()
{
	float value = getRightIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 8);
}

int isWallLeft()
{
	float value = getLeftIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 8);
}

void moveForward()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(762, mouse.velocity, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
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

void moveBackwardsAndCorrect()
{
	//Rotate Left 180
	setDirection(0, 1);
	straight(640, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
	
	
	//Go Backwards
	setDirection(1, 1);	
	straight(250, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
	
	setDirection(0, 0);
	straight(130, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void rotateLeft()
{
	//Rotate Left
	setDirection(0, 1);
	
	//Move Mouse
	straight(307, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void rotateRight()
{
	//Rotate Right
	setDirection(1, 0);
	
	//Move Mouse
	straight(320, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}


