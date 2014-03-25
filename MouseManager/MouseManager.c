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
#include "MouseManager_Sensors.h"

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
char firstTurn = 'n';

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
	
	updateSensors();
	updateSensors();
	
	setDirection(0, 0);
	updateWalls();
	//printMaze(&maze);
	straight(540, 0, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;
	
	/* INITIAL SEARCH */
	while(!(getDist(maze[mouse.x][mouse.y]) == 0))
	{
		updateWalls();		
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		if(firstTurn == 'R')
		{
			floodFill(maze, '7', mouse.x, mouse.y);
		}
		else
		{
			floodFill(maze, '6', mouse.x, mouse.y);
		}
		searchMove();
	}	
	
	floodFill(maze, firstTurn, mouse.x, mouse.y);

	/* RETURN */
	while(!(getDist(maze[mouse.x][mouse.y]) == 0))
	{
		updateWalls();
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		floodFill(maze, firstTurn, mouse.x, mouse.y);
		searchMove();
	} 
	
	if(firstTurn == 'R')
	{
		floodFill(maze, '9', mouse.x, mouse.y);
	}
	else
	{
		floodFill(maze, '8', mouse.x, mouse.y);
	}
	
	for(int i = 0; i < 2; i++)
	{
		/* FAST */
		while(!(getDist(maze[mouse.x][mouse.y]) == 0))
		{
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
			fastMove();
		}	
		
		floodFill(maze, firstTurn, mouse.x, mouse.y);

		while(!(getDist(maze[mouse.x][mouse.y]) == 0))
		{
			updateWalls();
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
			floodFill(maze, firstTurn, mouse.x, mouse.y);
			searchMove();
		}  
		
		if(firstTurn == 'R')
		{
			floodFill(maze, '9', mouse.x, mouse.y);
		}
		else
		{
			floodFill(maze, '8', mouse.x, mouse.y);
		}
	
	}
	
	stopMouse();
	
	while(!isButtonPushed(1));
		printMaze(maze);
		printWalls(maze);  
}


//Reset and start all mouse constants and timers
void initializeMouse()
{
	setupADC();	
	setupStepperMotor();
	startTimer();
	
	USART_init();
	
	mouse.velocity = 0;
	mouse.maxVelocity = 2500;
	mouse.acceleration = 3000;
	mouse.deceleration = 12000;

	enableDrive(1);
	turnOnTimers(1,1);
}

//Shut down motors from operating
void stopMouse()
{
	turnOnTimers(0, 0);
	enableDrive(0);	
}


