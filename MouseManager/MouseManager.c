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

**********************************************/

//Avr includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <math.h>

//Mouse Manager
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

int MOUSE_DIRECTION = 1;

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

	//Disable the mouse for now
	enableDrive(0);
	turnOnTimers(0, 0);
	
	int areWeSearching = UserInterfaceIntro();
	
	//Does the user want to skip the search phase and load a maze from EEPROM
	if(areWeSearching)
	{
		/* SEARCH MAZE */
		for(int i = 0; i < 10; i++)
		{
			turnOnLeds(7);
			_delay_ms(10);
			turnOnLeds(0);
			_delay_ms(90);
		}
		
		//Init Mouse
		enableDrive(1);
		turnOnTimers(1, 1);
		setDirection(0, 0);
		
		//Update Sensors
		updateSensors();
		updateSensors();	
		updateWalls();
		
		//Go Forward first block
		mouse.IR_LONG_CHECK_LEFT = 2;
		mouse.IR_LONG_CHECK_RIGHT = 2;
		straight(480, 0, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
		mouse.x += mouse.direction.x;
		mouse.y -= mouse.direction.y;	
		
		/* SEARCH */
		InitialSearchRun();
		
			//Search is Complete!
			updateWalls();
			
			/* TURN AROUND */
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
			StopFromSpeedHalf();
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
			moveBackwards();			
				
			//Save Maze to EEPROM
			saveCurrentMaze();
			writeMemByte(MOUSE_DIRECTION, firstTurn);
		
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
			moveForwardHalf();
			
			//Current Mouse Direction
			int dirx = mouse.direction.x;
			int diry = mouse.direction.y;
			
			//Reverse and go back
			mouse.direction.x  = -dirx;
			mouse.direction.y = -diry;
			
			//Set Position to next block
			mouse.x += mouse.direction.x;
			mouse.y -= mouse.direction.y;
		
		/* RETURN SEARCH*/
		ReturnSearchRun();
			
			//Turn Around
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
			StopFromSpeedHalf();
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
			moveBackwards();
		
			//Save Maze to EEPROM
			saveCurrentMaze();
			writeMemByte(MOUSE_DIRECTION, firstTurn);
		
		/* PICK UP AND PLACE MOUSE */
		enableDrive(0);
		waitForButtonPressed();
	}
	
	/* FAST RUN */
	FastRun();
	
		//Completed Search run, go back and search some more
		turnAroundInPlace();	
		
		floodFill(maze, firstTurn, mouse.x, mouse.y);
	
	/* RETURN */	
	ReturnSearchRun();
		
		//Turn Around
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		StopFromSpeedHalf();
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		moveBackwards();	
		
		//Save Maze to EEPROM
		saveCurrentMaze();
		writeMemByte(MOUSE_DIRECTION, firstTurn);
		
	stopMouse();
	
	while(!isButtonPushed(1));
		printMaze(maze);
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


int isJunction()
{
	long current = maze[mouse.x][mouse.y];
	char canWeGoStraight = !wallExists(current, mouse.direction.x, mouse.direction.y);
	char canWeGoLeft = !wallExists(current, -mouse.direction.y, mouse.direction.x);
	char canWeGoRight = !wallExists(current, mouse.direction.y, -mouse.direction.x);	

	if((canWeGoStraight&&!canWeGoLeft&&!canWeGoRight)||
	(!canWeGoStraight&&!canWeGoLeft&&canWeGoRight)||
	(!canWeGoStraight&&canWeGoLeft&&!canWeGoRight))
	return 0;
	else
	return 1;
}

void InitialSearchRun()
{
	/* INITIAL SEARCH */
	while(!(getDist(maze[mouse.x][mouse.y]) == 0))
	{
		updateWalls();		
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	

		if(isJunction()==1)
		{
			floodFill(maze, 'C', mouse.x, mouse.y);
			/* if(firstTurn == 'R')
			{
				floodFill(maze, '7', mouse.x, mouse.y);
			}
			else
			{
				floodFill(maze, '6', mouse.x, mouse.y);
			}*/
		}		 
		searchMove();
		
		stopAndFreezeWithButton();
	}
}

void ReturnSearchRun()
{
	turnOnLeds(0);	
	floodFill(maze, firstTurn, mouse.x, mouse.y);
	
	while(!(getDist(maze[mouse.x][mouse.y]) == 0))
	{
		updateWalls();
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		floodFill(maze, firstTurn, mouse.x, mouse.y);		
		searchMove();
		
		stopAndFreezeWithButton();
	} 
}


void FastRun()
{
	enableDrive(1);
	
	if(firstTurn == 'R')
	{	
		mouse.x = 15;
		mouse.y = 0;
		floodFill(maze, 'S', mouse.x, mouse.y);	
	}
	else
	{	
		mouse.x = 0;
		mouse.y = 0;
		floodFill(maze, 'S', mouse.x, mouse.y);
	}
	//printMaze(&maze);
	setDirection(0, 0);
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	straight(480, 0, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
	
	mouse.direction.y = -1;
	mouse.direction.x = 0;	
	
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;	
		
	/* FAST */
	while(!(getDist(maze[mouse.x][mouse.y]) == 0))
	{
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		fastMove();
	}	
}

void turnAroundInPlace()
{
	/* TURN AROUND */
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	StopFromSpeedHalf();
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	moveBackwards();
	
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	moveForwardHalf();
	
	//Current Mouse Direction
	int dirx = mouse.direction.x;
	int diry = mouse.direction.y;
	
	//Reverse and go back
	mouse.direction.x  = -dirx;
	mouse.direction.y = -diry;
	
	//Set Position to next block
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;
}

int UserInterfaceIntro()
{
	while(getFrontLeftIR() > 6)
	{
		int potValue = getPotSensorValue(0) / 256;
			
		if(potValue == 0)
			turnOnLeds(0);
		else if(potValue == 1)
			turnOnLeds(2);
		else if(potValue == 2)
			turnOnLeds(4);
		else if(potValue == 3)
			turnOnLeds(6);
			
		//Load EEPROM Maze if button is held for 2 seconds
		if(isButtonPushed(1))
		{
			char flag = 0;
		
			for(int i = 0; i < 25; i++)
			{
				turnOnLeds(7);
				_delay_ms(20);
				turnOnLeds(0);
				_delay_ms(20);	
				
				if(!isButtonPushed(1))
				{
					flag = 1;
					break;
				}
			}
			
			if(flag == 0)
			{
				for(int i = 0; i < 10; i++)
				{
					turnOnLeds(7);
					_delay_ms(10);
					turnOnLeds(0);
					_delay_ms(90);
				}
				readSavedMaze();				
				printlnNum(potValue);
				
				firstTurn = readMemByte(MOUSE_DIRECTION);
				mouse.speedMode = potValue;
				turnOnTimers(1, 1);
				
				return 0;
			}
		}
	}	

	return 1;
}

void waitForButtonPressed()
{
	while(!isButtonPushed(1))
	{
		turnOnLeds(7);
		_delay_ms(20);
		turnOnLeds(0);
		_delay_ms(20);
	}
	for(int i = 0; i < 10; i++)
	{
		turnOnLeds(7);
		_delay_ms(10);
		turnOnLeds(0);
		_delay_ms(90);
	}
}

void saveCurrentMaze()
{
	eeprom_update_block((const void*)maze, (void*)10, 1024);
}

void readSavedMaze()
{	
	eeprom_read_block((void*)maze, (const void*)10, 1024);
}

int readMemByte(int position)
{
	return eeprom_read_byte(position);
}

void writeMemByte(int position, int value)
{	
	return eeprom_update_byte(position, value);
}

void stopAndFreezeWithButton()
{
	if(isButtonPushed(1))
	{
		stopMouse();
		for(int i = 0; i < 25; i++)
		{
			turnOnLeds(7);
			_delay_ms(20);
			turnOnLeds(0);
			_delay_ms(20);
		}
		
		while(1==1)
		{
			if(isButtonPushed(1))
				printMaze(&maze);
				
			turnOnLeds(7);
			_delay_ms(20);
			turnOnLeds(0);
			_delay_ms(20);
		}
		
	}
}