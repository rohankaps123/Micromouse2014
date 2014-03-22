/**********************************************
 
  Micromouse 2014 Competition Code - "Venus"
 
             Mouse Manager.c
			 
	This code is designed to handle the decision
	making aspect of our mouse.  It combines the
	FloodFill and RobotModel files to make 
	decisions on which way the mouse should go.
 
Authors: 
    Justin Marple
    Rohan Kapoor
    Aaron Lucia
    Dylan Pare
    Aaron Dunbrack
	
Contact: jmarple@umass.edu
	
Date Started: March 12th 2014
Last Updated: March 16th 2014

**********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "MouseManager.h"
#include "MouseManager_Search.h"
#include "MouseManager_Fast.h"

//Mouse Profile
#include "RobotModel/RobotModel.h"
#include "RobotModel/RobotModel_Controls.h"
#include "RobotModel/RobotModel_StepperMotor.h"
#include "RobotModel/RobotModel_Sensors.h"

//Maze Solving
#include "FloodFill/FloodFill.h"
#include "FloodFill/FloodFill_BinaryOperations.h"
#include "FloodFill/FloodFill_Stack.h"
#include "FloodFill/FloodFill_Debug.h"

//Debugging
#include "USART.h"

//Our Mouse and Maze
volatile long maze[16][16];
volatile Mouse mouse;

void solveMaze()
{
	//stopMouse();
	setDirection(0, 0);
	mouse.IR_CORRECT_RIGHT = 40;
	int speed = 3000;
	
	straight(545, 0, speed, speed, 4000, 12000);
	
	while(1==1)
	{	
		setDirection(0, 0);
		
		if(isWallRight())
		{
			mouse.IR_CORRECT_RIGHT = 40;
		}		
		
		if(!isWallRight())
		{
			straight(381, mouse.velocity, speed, 0, 4000, 12000);
			rotateRight();
			setDirection(0, 0);
			straight(381, 0, speed, speed, 4000, 12000);
		}
		else if(isWallFront())
		{
			straight(381, mouse.velocity, speed, 0, 4000, 12000);
			mouse.IR_CORRECT_RIGHT = 0;
			mouse.IR_CORRECT_LEFT = 0;
			
			if(!isWallLeft())
			{
				rotateLeft();
				setDirection(0, 0);
				straight(381, 0, speed, speed, 4000, 12000);
			}
			else
			{
				moveBackwardsAndCorrect();
				setDirection(0, 0);
				straight(381, 0, speed, speed, 4000, 12000);
			}
		}
		else
		{
			setDirection(0, 0);
			straight(762, mouse.velocity, speed, speed, 4000, 12000);
		}
	
	}	
	//straight(762, mouse.velocity, speed, speed, 4000, 12000);
	straight(381, mouse.velocity, speed, 0, 4000, 12000);
	//Make the robot go forward
	/* setDirection(0, 0);
	int speed = 1500;
	
	if(isWallLeft())
		mouse.IR_CORRECT_LEFT = 30;
		
	straight(400, 0, speed, speed, 3000, 12000);
	
	while(!isWallFront())
	{
		if(isWallLeft())
		{
			mouse.IR_CORRECT_LEFT = 50;			
		}	
		else if(isWallRight())
		{
			mouse.IR_CORRECT_RIGHT = 0;
		}
		else
		{
			mouse.IR_CORRECT_RIGHT = 0;
			mouse.IR_CORRECT_LEFT = 0;	
		}	
			
		straight(762, mouse.velocity, speed, speed, 3000, 12000);
	}
	mouse.IR_CORRECT_LEFT = 0;
	mouse.IR_CORRECT_RIGHT = 0;
	straight(331, mouse.velocity, speed, 0, 3000, 12000);
		
	moveBackwardsAndCorrect(); */
	/* if(isWallFront())
	{
		float left = getFrontLeftIR();
		float right = getFrontRightIR();

		float distance = (left+right)/2;
		
		float offset = distance - 12.7;
		straight(offset * 100, mouse.velocity, speed, speed, 3000, 12000);
		
		if(!isWallRight())
		{
			float angle = getFrontAngle();

			turnOnLeds(7);
			
			smoothTurn(90 - angle/2, 6, mouse.velocity, speed, speed, 12000, 12000);
			straight(762, mouse.velocity, speed, 0, 3000, 12000); 
		}
		else if(!isWallLeft())
		{
			float angle = getFrontAngle();

			turnOnLeds(7);
			
			smoothTurn(-90-angle/2, 6, mouse.velocity, speed, speed, 12000, 12000);
			straight(762, mouse.velocity, speed, 0, 3000, 12000); 
		}
		else
		{	
			straight(331, mouse.velocity, speed, 0, 3000, 12000);
		
			moveBackwardsAndCorrect();
		}
	}
	else
	{
		straight(142, mouse.velocity, speed, speed, 3000, 12000);
		straight(381, mouse.velocity, speed, 0, 3000, 12000);
	} */
	
 	/* float left = getFrontLeftIR();
	float right = getFrontRightIR();

	float distance = (left+right)/2;
		
	float offset = distance - 13;
	
	straight(offset * 100, mouse.velocity, speed, speed, 3000, 12000);

	float angle = getFrontAngle();

	turnOnLeds(7);
	
	smoothTurn(90 - angle/2, 6, mouse.velocity, speed, speed, 12000, 12000);
	straight(762, mouse.velocity, speed, 0, 3000, 12000); 
	 */
	//straight(331, mouse.velocity, speed, 0, 3000, 12000);
	stopMouse();
	
	 
	/* //Reset maze to 0
	initializeMaze(&maze);
		
	//We Know at the starting point, there's something behind us
	setN(&maze[0][0], 1);
	mouse.direction.x = 0;
	mouse.direction.y = -1;
	mouse.x = 0;
	mouse.y = 0;
	
	while(1==1)
	{
		updateWalls();
		floodFill(maze, '6', mouse.x, mouse.y);
		searchMove();
		if(mouse.x == 5 && mouse.y == 5)
			break;
	}	
	
	floodFill(maze, 'S', mouse.x, mouse.y);
	 	
	stopMouse();
	
	while(!isButtonPushed(1));
		printMaze(maze);
		printWalls(maze);  */
}

int wallExists(long data, int dirx, int diry)
{
	if(dirx == 0 && diry == 1)	
		return getN(data);
	else if(dirx == 0 && diry == -1)
		return getS(data);
	else if(dirx == -1 && diry == 0)
		return getW(data);
	else if(dirx == 1 && diry == 0)
		return getE(data);
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

void updateWalls()
{	
	print("Mousex: ");printInt(mouse.x);
	print(", Mousey: ");printInt(mouse.y);
	print("\n\r");
	int front = isWallFront();
	int left = isWallLeft();
	int right = isWallRight();
	int dirx = mouse.direction.x;
	int diry = mouse.direction.y;
	int N = 0;
	int E = 0;
	int S = 0;
	int W = 0;

	//Translate our robot's sensors to the maze coordinates
	if(dirx == 0 && diry == 1)
	{
		N = front;
		E = right;
		W = left;
	}
	else if(dirx == 0 && diry == -1)
	{
		S = front;
		W = right;
		E = left;
	}
	else if(dirx == 1 && diry == 0)
	{
		E = front;
		S = right;
		N = left;
	}
	else if(dirx == -1 && diry == 0)
	{
		W = front;
		N = right;
		S = left;
	}
	
	//Update Bits that have turned to 1
	if(N) setN(&maze[mouse.x][mouse.y], N);	
	if(W) setW(&maze[mouse.x][mouse.y], W);		
	if(S) setS(&maze[mouse.x][mouse.y], S);		
	if(E) setE(&maze[mouse.x][mouse.y], E);
}
