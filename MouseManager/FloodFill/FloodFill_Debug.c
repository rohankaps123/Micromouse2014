#include "FloodFill_Debug.h"
#include "FloodFill_BinaryOperations.h"

//Print the maze onto a console via USART
void printMaze(long maze[16][16])
{
	//Print Maze Status
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			print("[");
			printInt(getDist(maze[j][i]));
			print("],");
		}
		print(" BREAK\n\r");
	}
}

//Print all the walls found onto a console via USART
void printWalls(long maze[16][16])
{
	print(":: Mouse Wall View :: \n\r");
	//Y Coord
	for(int i = 0; i < 16; i++)
	{	
		//Up, side and bottom walls
		for(int q = 0; q < 3; q++)
		{
			//X Coord
			for(int j = 0; j < 16; j++)
			{		
				//Up Walls
				if(q == 0)
				{
					print(" ");
					if(getN(maze[j][i]))
						print("-");
					else
						print(" ");
					print(" ");
				}
				else if(q == 1)
				{
					if(getW(maze[j][i]))
						print("|");
					else
						print(" ");
						
					print(" ");
					if(getE(maze[j][i]))
						print("|");
					else
						print(" ");
				}
				else if(q == 2)
				{
					print(" ");
					if(getS(maze[j][i]))
						print("-");
					else
						print(" ");
					print(" ");
				}
			}		
			print("\n\r");
		}
	}
	print(":: End View :: \n\r");
	
}

//Demo state that doesn't use the FloodFill Methods
void wallFallowingSolve()
{	
	/* for(int i = 0; i < 10; i++)
	{		
		updateWalls();
		
		int right = isWallRight();
		int front = isWallFront();
		int left = isWallLeft();

		if(front)
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
		
		float angle = getFrontAngle();
		
		//int lFront = getFrontLeftIR();
		//int rFront = getFrontRightIR();
		
		
		if(!right)
		{
			if(front)
				rotateRightWithFix(angle);
			else
				rotateRight();
		}
		else if(front && !left)
		{
			rotateLeftWithFix(angle);
		}
		else if(front)
		{
			moveBackwardsAndCorrect();
		}	
		
		if(left && right)		
		{
			mouse.IR_CORRECT = 30;			
		}
		else if(left)
		{
			mouse.IR_CORRECT_LEFT = 30;
		}
		else if(right)
		{
			mouse.IR_CORRECT_RIGHT = 30;
		}
		else
		{
			//turnOnLeds(0);
		}
		
		moveForwardAndStop();
		
		mouse.IR_CORRECT = 0;	
		mouse.IR_CORRECT_LEFT = 0;
		mouse.IR_CORRECT_RIGHT = 0;
	} */
}
