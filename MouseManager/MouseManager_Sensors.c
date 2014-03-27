#include <avr/io.h>
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

#include "USART.h"

//Our Mouse and Maze
extern volatile long maze[16][16];
extern volatile Mouse mouse;
extern char firstTurn;

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

void updateWalls()
{	
	int front = isWallFront();
	int left = isWallLeft();
	int right = isWallRight();
	
/* 	if(front)
	{
		turnOnLeds(7);
		printNum(mouse.sensor[LEFT_FRONT_IR].value);
		print(" , ");
		printNum(mouse.sensor[RIGHT_FRONT_IR].value);
		print("\n\r");
	}		
	else
	{
		turnOnLeds(0);
	} */
		
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
	
		
	//if we're still checking for mouse position
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

	setExp(&maze[mouse.x][mouse.y], 1);
}