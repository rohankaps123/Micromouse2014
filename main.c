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

//Debugging
#include "USART.h"

volatile unsigned long milliseconds;
extern volatile Mouse mouse;
extern volatile int cells[16][16];
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
	//2.8
	for(int i = 0; i < 30; i++)
	{		
		int right = isWallRight();
		int front = isWallFront();
		int left = isWallLeft();
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
			mouse.IR_CORRECT = 20;
			turnOnLeds(1);
		}
		else
		{
			turnOnLeds(0);
		}
		moveForwardAndStop();
		
		mouse.IR_CORRECT = 0;		
	}
	
	/*
	
	printlnNum(angle);
	
	rotateRightWithFix(angle);
	*/
	
	turnOnTimers(0, 0);
	enableDrive(0);
	
	while(1==1)
	{
		printNum((float)getFrontLeftIR());
		print(",");
		printlnNum((float)getFrontRightIR());	
		
	}	
}
