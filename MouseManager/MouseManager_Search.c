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
	
	char dStraight = getDist(cstraight);
	char dLeft = getDist(left);
	char dRight = getDist(right);
	
	char canWeGoStraight = !wallExists(current, mouse.direction.x, mouse.direction.y);
	char canWeGoLeft = !wallExists(current, -mouse.direction.y, mouse.direction.x);
	char canWeGoRight = !wallExists(current, mouse.direction.y, -mouse.direction.x);	 
	 
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
		//Correct Using Walls?
		if(!canWeGoRight)
		{
			mouse.IR_CORRECT_RIGHT = 40;
		}
		else if(!canWeGoLeft)
		{
			mouse.IR_CORRECT_LEFT = 40;
		}
		
		GoForwardOneBlock();
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
		rotateLeftWithFix(angle);
	}
	else
	{
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
		rotateRightWithFix(angle);
	}
	else
	{
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
		
	float left = getFrontLeftIR();
	float right = getFrontRightIR();
	float length = (left+right)/2;
	
	length = length - 3.5;
	
	printlnNum((float)length);
	
	if(length < 0)
	{
		setDirection(1, 1);
		length = -length;
	}
	
	straight((length)*42, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}