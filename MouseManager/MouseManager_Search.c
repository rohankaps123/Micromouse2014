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

#include "USART.h"

//Our Mouse and Maze
extern volatile long maze[16][16];
extern volatile Mouse mouse;

void searchMove()
{
	long current = maze[mouse.x][mouse.y];
	long cstraight = maze[mouse.x + mouse.direction.x][mouse.y - mouse.direction.y];
	long left = maze[mouse.x - mouse.direction.y][mouse.y - mouse.direction.x];
	long right = maze[mouse.x + mouse.direction.y][mouse.y + mouse.direction.x];
	long back = maze[mouse.x + mouse.direction.x][mouse.y + mouse.direction.y];
	
	char dStraight = getDist(cstraight);
	char dLeft = getDist(left);
	char dRight = getDist(right);	
	char dBack = getDist(back);
	
	char canWeGoStraight = !wallExists(current, mouse.direction.x, mouse.direction.y);
	char canWeGoLeft = !wallExists(current, -mouse.direction.y, mouse.direction.x);
	char canWeGoRight = !wallExists(current, mouse.direction.y, -mouse.direction.x);	 
		
	char isExploredStraight = getExp(cstraight);
	char isExploredLeft = getExp(left);
	char isExploredRight = getExp(right);
	
	/* All Directions Blocked*/
	if(!canWeGoStraight && !canWeGoLeft && !canWeGoRight)
	{
		mouse.IR_LONG_CHECK_LEFT = 2;
		mouse.IR_LONG_CHECK_RIGHT = 2;
		StopAndGoBack();
	}	
	/* Forward is Most Optimal*/
	else if( 
		canWeGoStraight && 
		((dStraight <= dLeft) || (!canWeGoLeft)) &&
		((dStraight <= dRight) || (!canWeGoRight)) &&
		(isExploredRight >= isExploredStraight || (!canWeGoRight) || (dStraight!=dRight)) && 
		(isExploredLeft >= isExploredStraight || (!canWeGoLeft) || (dStraight!=dLeft)) ) 
	{
		//Correct Using Walls?
		if(!canWeGoRight)
		{
			mouse.IR_CORRECT_RIGHT = 40;	
		}
		else if(!canWeGoLeft)
		{
			mouse.IR_CORRECT_LEFT = 40;
		}
		
		if(!canWeGoRight)
		{
			mouse.IR_LONG_CHECK_RIGHT = 1;
			mouse.IR_LONG_OFF_DISTANCE_RIGHT = mouse.rightMotor.totalCount;
		}
		else
		{
			mouse.IR_LONG_CHECK_RIGHT = 2;
		}
		if(!canWeGoLeft)
		{
			mouse.IR_LONG_CHECK_LEFT = 1;
			mouse.IR_LONG_OFF_DISTANCE_LEFT = mouse.leftMotor.totalCount;
		}
		else
		{
			mouse.IR_LONG_CHECK_LEFT = 2;
		}
		
		GoForwardOneBlock();
		
		if(mouse.IR_LONG_CHECK_RIGHT == 1)
			mouse.IR_LONG_CHECK_RIGHT = 2;
		if(mouse.IR_LONG_CHECK_LEFT == 1)
			mouse.IR_LONG_CHECK_LEFT = 2;
		
	}
	/* Left is Most Optimal */
	else if(
		canWeGoLeft && 
		((dLeft <= dStraight) || (!canWeGoStraight)) &&
		((dLeft <= dRight) || (!canWeGoRight))  &&
		(isExploredRight >= isExploredLeft || (!canWeGoRight) || (dLeft != dRight)) && 
		(isExploredStraight >= isExploredLeft || (!canWeGoStraight) || (dStraight != dLeft)) )
	{	
		RotateLeft(canWeGoStraight);
		mouse.IR_LONG_CHECK_RIGHT = 2;
		mouse.IR_LONG_CHECK_LEFT = 2;
	}
	/* Right is Most Optimal */
	else if(
		canWeGoRight &&
		((dRight <= dStraight) || (!canWeGoStraight)) &&
		((dRight <= dLeft) || (!canWeGoLeft)) &&
		(isExploredLeft >= isExploredRight || (!canWeGoLeft) || (dRight != dLeft)) && 
		(isExploredStraight >= isExploredRight || (!canWeGoStraight) || (dRight != dStraight)) )
	{		
		RotateRight(canWeGoStraight);
		mouse.IR_LONG_CHECK_RIGHT = 2;
		mouse.IR_LONG_CHECK_LEFT = 2;
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

/* GO FORWARD */
void GoForwardOneBlock()
{	
	//Go forward
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;
	
	moveForward();
}

void RotateLeft(int canWeGoStraight)
{
	//Current Mouse Direction
	int dirx = mouse.direction.x;
	int diry = mouse.direction.y;
	
	//Stop At Midpoint in Cell
	StopFromSpeedHalf();
	
	//Rotate In Place
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	if(!canWeGoStraight)
	{
		fixLength();
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;		
		float angle = getFrontAngle();
		if(angle > 20)
			angle = 20;
		else if(angle < -20)
			angle = -20;
		rotateLeftWithFix(angle);
	}
	else
	{
		int dist = mouse.leftMotor.totalCount - mouse.IR_LONG_OFF_DISTANCE_LEFT - 600;
		if(dist > 150)
			dist = 150;
		else if(dist < -150)
			dist = -150;
			
		if(mouse.IR_LONG_CHECK_LEFT == 0)
		{			
			if(dist < 0)
			{
				setDirection(0, 0);
				dist = -dist;
			}
			else
			{
				setDirection(1, 1);
			}
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		
			straight(dist, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration); 
			
			mouse.IR_LONG_CHECK_LEFT = 2;
		} 
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		rotateLeft();
	}
	
	//Start Moving to next cell
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	moveForwardHalf();
	
	//Turn left
	mouse.direction.x = -diry;
	mouse.direction.y = dirx;
	
	//Set Position to next block
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;
}

void RotateRight(int canWeGoStraight)
{	
	//Current Mouse Direction
	int dirx = mouse.direction.x;
	int diry = mouse.direction.y;

	//Stop at Midpoint in Cell
	StopFromSpeedHalf();	

	//Rotate In Place
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;		
	
	if(!canWeGoStraight)
	{
		fixLength();
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;		
		float angle = getFrontAngle();
		if(angle > 20)
			angle = 20;
		else if(angle < -20)
			angle = -20;
		rotateRightWithFix(angle);
	}
	else
	{		
		int dist = mouse.rightMotor.totalCount - mouse.IR_LONG_OFF_DISTANCE_RIGHT - 600;
		if(dist > 150)
			dist = 150;
		else if(dist < -150)
			dist = -150;
			
		if(mouse.IR_LONG_CHECK_RIGHT == 0)
		{			
			if(dist < 0)
			{
				setDirection(0, 0);
				dist = -dist;
			}
			else
			{
				setDirection(1, 1);
			}
			mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		
			straight(dist, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration); 
			
			mouse.IR_LONG_CHECK_RIGHT = 2;
		} 
		mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		rotateRight();
	}
	
	//Move Forward to next cell
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	moveForwardHalf();
	
	//Turn Right
	mouse.direction.x = diry;
	mouse.direction.y = -dirx;
	
	//Set Position to next block
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;
}

void StopAndGoBack()
{
	//Current Mouse Direction
	int dirx = mouse.direction.x;
	int diry = mouse.direction.y;
	
	//Stop at Midpoint in Cell
	StopFromSpeedHalf();
	
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	
	moveBackwardsAndCorrect();
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	moveForwardHalf();
	
	//Reverse and go back
	mouse.direction.x  = -dirx;
	mouse.direction.y = -diry;
	
	//Set Position to next block
	mouse.x += mouse.direction.x;
	mouse.y -= mouse.direction.y;
}

void fixLength()
{
	setDirection(0, 0);
	
	updateSensors();
	float left = mouse.sensor[2].value;//getFrontLeftIR();
	float right = mouse.sensor[3].value;//getFrontRightIR();
	float length = (left+right)/2;
	
	length = length - 3.5;
	
/* 	print("LEFT = ");
	printNum(left);
	print("\t RIGHT = ");
	printNum(right);
	print("\t LEN = ");
	printlnNum((float)length);  */
	
	if(length < 0)
	{
		setDirection(1, 1);
		length = -length;
	}
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	straight((length)*42, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}