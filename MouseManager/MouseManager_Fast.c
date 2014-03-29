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

#include "USART.h"

//Our Mouse and Maze
extern volatile long maze[16][16];
extern volatile Mouse mouse;

void fastMove()
{
	int countStraight=0;
	
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

	mouse.IR_LONG_CHECK_LEFT = 2;
	mouse.IR_LONG_CHECK_RIGHT = 2;
	
	/* All Directions Blocked*/
	if(!canWeGoStraight && !canWeGoLeft && !canWeGoRight)
	{
		StopAndGoBack();
	}
	/* Forward is Most Optimal*/
	else if( 
		canWeGoStraight && 
		((dStraight <= dLeft) || (!canWeGoLeft)) &&
		((dStraight <= dRight) || (!canWeGoRight)) ) 
	{
		print("STRAIGHT(S)\n\r");
		//Correct Using Walls?
		if(!canWeGoRight)
		{
			mouse.IR_CORRECT_RIGHT = 70;
		}
		else if(!canWeGoLeft)
		{
			mouse.IR_CORRECT_LEFT = 70;
		}
		
		int a = mouse.x;
		int b = mouse.y;
		
		while(1==1)
		{
			if(getNextMove(a,b)==2)
			{
				countStraight++;
				a += mouse.direction.x;
				b -= mouse.direction.y;
			}
			else if(getNextMove(a, b) == 0)
			{
				countStraight++;
				a += mouse.direction.x;
				b -= mouse.direction.y;
				break;
			}
			else
			{
				break;
			}
		}
				
		printlnNum(countStraight);
		moveForwardBlocks(countStraight);
		mouse.x += mouse.direction.x*countStraight;
		mouse.y -= mouse.direction.y*countStraight;		
	}
	/* Left is Most Optimal */
	else if(
		canWeGoLeft && 
		((dLeft <= dStraight) || (!canWeGoStraight)) &&
		((dLeft <= dRight) || (!canWeGoRight)) )
	{	
		print("LEFT\n\r");		
		RotateLeft(canWeGoStraight);
	}
	/* Right is Most Optimal */
	else if(
		canWeGoRight &&
		((dRight <= dStraight) || (!canWeGoStraight)) &&
		((dRight <= dLeft) || (!canWeGoLeft)) )
	{	
		print("RIGHT\n\r");
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


int getNextMove(int a, int b){
	long current = maze[a][b];
	long cstraight = maze[a + mouse.direction.x][b - mouse.direction.y];
	long left = maze[a - mouse.direction.y][b - mouse.direction.x];
	long right = maze[a + mouse.direction.y][b + mouse.direction.x];
	
	char dStraight = getDist(cstraight);
	char dLeft = getDist(left);
	char dRight = getDist(right);
	
	char canWeGoStraight = !wallExists(current, mouse.direction.x, mouse.direction.y);
	char canWeGoLeft = !wallExists(current, -mouse.direction.y, mouse.direction.x);
	char canWeGoRight = !wallExists(current, mouse.direction.y, -mouse.direction.x);	 

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
