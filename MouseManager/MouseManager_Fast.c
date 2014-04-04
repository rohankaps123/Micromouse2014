/*
 - Micromouse Speed Run Controller
   - Determines next move based off of maze layout
   - Optimizes to increase speeds on straight aways
*/

//AVR Includes
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

//Mouse Manager
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
extern volatile long maze[16][16];
extern volatile Mouse mouse;

//Function that determines the next move for a search run
void fastMove()
{
	int countStraight=0;
	
	//Get current maze cells
	long current = maze[mouse.x][mouse.y];
	long cstraight = maze[mouse.x + mouse.direction.x][mouse.y - mouse.direction.y];
	long left = maze[mouse.x - mouse.direction.y][mouse.y - mouse.direction.x];
	long right = maze[mouse.x + mouse.direction.y][mouse.y + mouse.direction.x];
	
	//Get distances in each cell
	char dStraight = getDist(cstraight);
	char dLeft = getDist(left);
	char dRight = getDist(right);
	
	//Find where walls are so we know where we can go
	char canWeGoStraight = !wallExists(current, mouse.direction.x, mouse.direction.y);
	char canWeGoLeft = !wallExists(current, -mouse.direction.y, mouse.direction.x);
	char canWeGoRight = !wallExists(current, mouse.direction.y, -mouse.direction.x);	 

	//Turn off wall checking by default
	mouse.IR_LONG_CHECK_LEFT = 2;
	mouse.IR_LONG_CHECK_RIGHT = 2;
	
	/* All Directions Blocked*/
	if(!canWeGoStraight && !canWeGoLeft && !canWeGoRight)
	{
		//Reverse (shouldn't happen in fast run)
		StopAndGoBack();
	}
	/* Forward is Most Optimal*/
	else if( 
		canWeGoStraight && 
		((dStraight <= dLeft) || (!canWeGoLeft)) &&
		((dStraight <= dRight) || (!canWeGoRight)) ) 
	{
		//Correct Using Walls?
		if(!canWeGoRight)
		{
			mouse.IR_CORRECT_RIGHT = 70;
		}
		else if(!canWeGoLeft)
		{
			mouse.IR_CORRECT_LEFT = 70;
		}
			
		//Optimize straight, count how many blocks we can go forward
		int a = mouse.x;
		int b = mouse.y;
		
		while(1==1)
		{
			//If we can go straight still
			if(getNextMove(a,b)==2)
			{
				countStraight++;
				a += mouse.direction.x;
				b -= mouse.direction.y;
			}
			//If we are at center, add then break
			else if(getNextMove(a, b) == 0)
			{
				countStraight++;
				a += mouse.direction.x;
				b -= mouse.direction.y;
				break;
			}
			//Break due to turn
			else
			{
				break;
			}
		}
		
		//Move forward that many blocks
		moveForwardBlocks(countStraight);
		
		//Update mouse position
		mouse.x += mouse.direction.x*countStraight;
		mouse.y -= mouse.direction.y*countStraight;		
	}
	/* Left is Most Optimal */
	else if(
		canWeGoLeft && 
		((dLeft <= dStraight) || (!canWeGoStraight)) &&
		((dLeft <= dRight) || (!canWeGoRight)) )
	{		
		RotateLeft(canWeGoStraight);
	}
	/* Right is Most Optimal */
	else if(
		canWeGoRight &&
		((dRight <= dStraight) || (!canWeGoStraight)) &&
		((dRight <= dLeft) || (!canWeGoLeft)) )
	{	
		RotateRight(canWeGoStraight);
	}
	else
	{
		stopMouse();
	}
	
	//Reset All IR Corrections to off
	mouse.IR_CORRECT = 0;
	mouse.IR_CORRECT_LEFT = 0;
	mouse.IR_CORRECT_RIGHT = 0;
}


int getNextMove(int a, int b)
{
	//Get current maze cells
	long current = maze[a][b];
	long cstraight = maze[a + mouse.direction.x][b - mouse.direction.y];
	long left = maze[a - mouse.direction.y][b - mouse.direction.x];
	long right = maze[a + mouse.direction.y][b + mouse.direction.x];
	
	//Get distances in each cell
	char dStraight = getDist(cstraight);
	char dLeft = getDist(left);
	char dRight = getDist(right);
	
	//Find where walls are so we know where we can go	
	char canWeGoStraight = !wallExists(current, mouse.direction.x, mouse.direction.y);
	char canWeGoLeft = !wallExists(current, -mouse.direction.y, mouse.direction.x);
	char canWeGoRight = !wallExists(current, mouse.direction.y, -mouse.direction.x);	 

	//Center
	if(dStraight == 0 && canWeGoStraight)
	{	
		return 0;
	}
	
	/* All Directions Blocked*/
	if(!canWeGoStraight && !canWeGoLeft && !canWeGoRight)
	{
		return 1;
	}
	/* Forward is Most Optimal*/
	else if( 
		canWeGoStraight && 
		((dStraight <= dLeft) || (!canWeGoLeft)) &&
		((dStraight <= dRight) || (!canWeGoRight)) ) 
	{

		
		return 2;
	}
	/* Left is Most Optimal */
	else if(
		canWeGoLeft && 
		((dLeft <= dStraight) || (!canWeGoStraight)) &&
		((dLeft <= dRight) || (!canWeGoRight)) )
	{	
		
		return 3;
	}
	/* Right is Most Optimal */
	else if(
		canWeGoRight &&
		((dRight <= dStraight) || (!canWeGoStraight)) &&
		((dRight <= dLeft) || (!canWeGoLeft)) )
	{		
		return 4;
	}
	else
	{
		return 5;
	}

}
