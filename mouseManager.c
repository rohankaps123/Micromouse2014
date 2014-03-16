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
Last Updated: March 15th 2014

**********************************************/

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
	//Reset maze to 0
	initializeMaze(&maze);
		
	//We Know at the starting point, there's something behind us
	setN(&maze[0][0], 1);
	mouse.direction.x = 0;
	mouse.direction.y = -1;
	mouse.x = 0;
	mouse.y = 0;
	
	for(int i = 0; i < 40; i++)
	{
		turnOnTimers(0, 0);
		enableDrive(0);
		updateWalls();
		floodFill(maze, 'C');
		turnOnTimers(1, 1);
		enableDrive(1);
		determineNextMove();
	}	
	
	stopMouse();
	
	while(!isButtonPushed(1));
		printMaze(maze);
		printWalls(maze);
}

void determineNextMove()
{
	long current = maze[mouse.x][mouse.y];
	long cstraight = maze[mouse.x + mouse.direction.x][mouse.y - mouse.direction.y];
	long left = maze[mouse.x - mouse.direction.y][mouse.y - mouse.direction.x];
	long right = maze[mouse.x + mouse.direction.y][mouse.y + mouse.direction.x];
	
	char dStraight = getDist(cstraight);
	char dLeft = getDist(left);
	char dRight = getDist(right);
	
	char canWeGoStraight = !wallExists(current, mouse.direction.x, mouse.direction.y);
	char canWeGoLeft = !wallExists(current, -mouse.direction.y, mouse.direction.x);
	char canWeGoRight = !wallExists(current, mouse.direction.y, -mouse.direction.x);

	int dirx = mouse.direction.x;
	int diry = mouse.direction.y;
	
	//Correct for straight distance
	if(!canWeGoStraight)
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
	
	if(!canWeGoStraight && !canWeGoLeft && !canWeGoRight)
	{
		//Reverse and go back
		mouse.direction.x  = -dirx;
		mouse.direction.y = -diry;
		moveBackwardsAndCorrect();
	}
	else if( 
		canWeGoStraight && 
		((dStraight <= dLeft) || (!canWeGoLeft)) &&
		((dStraight <= dRight) || (!canWeGoRight)) )
	{
		//Go forward
	}
	else if(
		canWeGoLeft && 
		((dLeft <= dStraight) || (!canWeGoStraight)) &&
		((dLeft <= dRight) || (!canWeGoRight)) )
	{	
		//Turn left
		mouse.direction.x = -diry;
		mouse.direction.y = dirx;
	
		if(!canWeGoStraight)
		{
			float angle = getFrontAngle();
			rotateLeftWithFix(angle);
		}
		else
		{
			rotateLeft();
		}
	}
	else if(
		canWeGoRight &&
		((dRight <= dStraight) || (!canWeGoStraight)) &&
		((dRight <= dLeft) || (!canWeGoLeft)) )
	{		
		//Turn Right
		mouse.direction.x = diry;
		mouse.direction.y = -dirx;
		
		if(!canWeGoStraight)
		{
			float angle = getFrontAngle();
			rotateRightWithFix(angle);
		}
		else
		{
			rotateRight();
		}
	}

	if(!canWeGoLeft && !canWeGoRight)
	{
		mouse.IR_CORRECT = 30;
	}
	else if(!canWeGoLeft)
	{
		mouse.IR_CORRECT_LEFT = 30;
	}
	else if(!canWeGoRight)
	{
		mouse.IR_CORRECT_RIGHT = 30;
	}
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;
	moveForwardAndStop();	
	
	mouse.IR_CORRECT = 0;
	mouse.IR_CORRECT_LEFT = 0;
	mouse.IR_CORRECT_RIGHT = 0;
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
