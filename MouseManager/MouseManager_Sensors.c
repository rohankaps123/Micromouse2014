/*****************************
   MouseManager_Sensors.c

Detect and update walls in the
maze using our IR sensors on 
the mouse
   
*****************************/

#include <avr/io.h>
#include <util/delay.h>
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

//Our Mouse and Maze
extern volatile long maze[16][16];
extern volatile Mouse mouse;
extern char firstTurn;

/* This function returns true/false if there is a wall stored in the maze in a certain direction */
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

/* This function updates the wall data in the current cell */
void updateWalls()
{	
	//Get what the mouse see's at this moment
	int front = isWallFront();
	int left = isWallLeft();
	int right = isWallRight();
	int dirx = mouse.direction.x;
	int diry = mouse.direction.y;
	
	//The variables to see the maze to
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
	
		
	//Do we need to flip the (0,y) side to (15, y) and clear (0, y)
	if(firstTurn == 'n')
	{
		//If we need to flip sides
		if(!W)
		{			
			mouse.x = 15;
			setS(&maze[mouse.x][mouse.y], getS(maze[0][mouse.y]));
			setS(&maze[0][mouse.y], 0);
			for(int i=0;i<15;i++)
			{
				setExp(&maze[15][i], getExp(&maze[0][i]));
				setExp(&maze[0][i], 0);
				setW(&maze[15][i],getE(maze[0][i]));
				setE(&maze[0][i],0); 
			}
			
			firstTurn = 'R';
			
		}
		else if(!E)
		{
			firstTurn = 'L';
		}
	}
	
	//Update Bits that have turned to 1
	if(N) setN(&maze[mouse.x][mouse.y], N);	
	if(W) setW(&maze[mouse.x][mouse.y], W);		
	if(S) setS(&maze[mouse.x][mouse.y], S);		
	if(E) setE(&maze[mouse.x][mouse.y], E);

	//Set this cell as explored
	setExp(&maze[mouse.x][mouse.y], 1);
}