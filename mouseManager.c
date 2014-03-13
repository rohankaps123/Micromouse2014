#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "mouseManager.h"

//Mouse Profile
#include "RobotModel/RobotModel.h"
#include "RobotModel/RobotModel_Controls.h"
#include "RobotModel/RobotModel_StepperMotor.h"
#include "RobotModel/RobotModel_Sensors.h"

//Maze Solving
#include "FloodFill/DataComponents.h"
#include "FloodFill/floodfill.h"

//Debugging
#include "USART.h"

#define MAZEHEIGHT 16
#define MAZEWIDTH 16

char Destination='C';

//Our Maze
volatile long maze[16][16];

//Our Mouse
volatile Mouse mouse;

void solveMaze()
{
	//Reset maze to 0
	initializeMaze(maze);
	
	floodFill(maze, Destination);
	
	//Print Maze Status
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			print("[");
			printInt(getDist(maze[j][i]));
			print("],");
		}
		print(" BREAK\n\r");
	}
	
}

//Reset and start all mouse constants and timers
void initializeMouse()
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
}

//Shut down motors from operating
void stopMouse()
{
	turnOnTimers(0, 0);
	enableDrive(0);	
}

void wallFallowingSolve()
{
	for(int i = 0; i < 30; i++)
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
		
		//int lFront = getFrontLeftIR();
		//int rFront = getFrontRightIR();
		
		
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
			//turnOnLeds(0);
		}
		
		moveForwardAndStop();
		
		mouse.IR_CORRECT = 0;	
		mouse.IR_CORRECT_LEFT = 0;
		mouse.IR_CORRECT_RIGHT = 0;
	}
}
