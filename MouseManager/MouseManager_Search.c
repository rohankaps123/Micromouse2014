#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

//Mouse Manager
#include "MouseManager.h"
#include "MouseManager_Search.h"

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

//Our Mouse and Maze
extern volatile long maze[16][16];
extern volatile Mouse mouse;

void searchMove()
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