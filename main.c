/**********************************************
 
  Micromouse 2014 Competition Code - "Venus"
 
     University Of Massachusetts Amherst

 
Authors: 
    Justin Marple
    Rohan Kapoor
    Aaron Lucia
    Dylan Pare
    Aaron Dunbrack
	
Contact: jmarple@umass.edu
	
Date Started: Feburary 3rd 2014
Last Updated: March 1st 2014

**********************************************/

//Basic Avr-gcc includes
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

//Mouse Profile
#include "RobotModel/RobotModel.h"
#include "RobotModel/RobotModel_Controls.h"
#include "RobotModel/RobotModel_StepperMotor.h"
#include "RobotModel/RobotModel_Sensors.h"

//Maze Solving
#include "FloodFill/DataComponents.h"

//Debugging
#include "USART.h"

volatile unsigned long milliseconds;
extern volatile Mouse mouse;

extern volatile int gyroSum;
extern volatile int gyroComp;

int main(void)
{
	setupADC();	
	setupStepperMotor();
	startTimer();
	
	
	USART_init();
	
	mouse.velocity = 0;
	mouse.maxVelocity = 5000;
	mouse.acceleration = 2000;
	mouse.deceleration = 10000;

	enableDrive(1);
	turnOnTimers(1,1);
	
	int var = 0;
	
	setDistance(&var, 7);
	setN(&var, 1);
	
	printlnNum(getN(var));
	
	//smoothTurn(90, 9, 0, 6000, 0, 3000, 3000);
	/*for(int i = 0; i < 30; i++)
	{		
		int right = isWallRight();
		int front = isWallFront();
		int left = isWallLeft();

		if(front)
		{
			float left = getFrontLeftIR();
			float right = getFrontRightIR();
			float length = (left+right)/2;
			
			length = length - 3;
			if(length < 0)
			{
				setDirection(1, 1);
				length = -length;
			}
			straight((length)*42, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
			
		}
		
		float angle = getFrontAngle();
		
		int lFront = getFrontLeftIR();
		int rFront = getFrontRightIR();
		
		
		if(!right)
		{
			if(front)
				rotateRightWithFix(angle);
			else
				rotateRight();
		}
		else if(front && !left)
		{
			rotateLeftWithFix(angle);
		}
		else if(front)
		{
			moveBackwardsAndCorrect();
		}	
		
		if(left && right)		
		{
			mouse.IR_CORRECT = 30;			
		}
		else if(left)
		{
			mouse.IR_CORRECT_LEFT = 30;
		}
		else if(right)
		{
			mouse.IR_CORRECT_RIGHT = 30;
		}
		else
		{
			turnOnLeds(0);
		}
		
		moveForwardAndStop();
		
		mouse.IR_CORRECT = 0;	
		mouse.IR_CORRECT_LEFT = 0;
		mouse.IR_CORRECT_RIGHT = 0;
	}*/
	
	turnOnTimers(0, 0);
	enableDrive(0);	

	while(1==1){}	
}