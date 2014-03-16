/**********************************************
 
  Micromouse 2014 Competition Code - "Venus"
 
     University Of Massachusetts Amherst

 
Authors: 
    Justin Marple
    Rohan Kapoor
    Aaron Lucia
    Dylan Pare
    Aaron Dunbrack
	
Contact: jmarple@umass.edu
	
Date Started: Feburary 3rd 2014
Last Updated: March 1st 2014

**********************************************/

//Basic Avr-gcc includes
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "Floodfill.h"
#include "FloodFill_BinaryOperations.h"
#include "FloodFill_Stack.h"
#include "FloodFill_Debug.h"

//Debugging
#include "USART.h"

//These will hold which cells to add distances to
struct llstack stack1;
struct llstack stack2;
	
//Refresh Distances in FloodFill Maze
void floodFill(long mazecells[16][16], char goal)
{
	//What is the current number?
	int level=0;
	
	//Reset Stack to null
	init_stack(&stack1);
	init_stack(&stack2);
	
	//Reset Maze distances!!
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			setDistance(&mazecells[i][j], 255);
		}
	}
	
	//If our goal is the center..
	if(goal == 'C')
	{
		//Set our center to "0"
		push(mazecells[7][7], &stack1);		
		push(mazecells[7][8], &stack1);		
		push(mazecells[8][7], &stack1);		
		push(mazecells[8][8], &stack1);
	}
	//Otherwise, default to start point
	else
	{
		push(mazecells[0][0], &stack1);
	}

	while(1==1)
	{	
		while(!stackIsEmpty(&stack1))
		{	
			//Get value of value on stack
			long temp = top(&stack1);
			int x = getX(temp);
			int y = getY(temp);				
			setDistance(&temp, level);
			
			//Get current(non-updated) distance from cell
			int dist = getDist(mazecells[x][y]);	
			
			//Save Data to Cell
			if(dist == 255)
			{
				mazecells[x][y] = temp;
				
				if(x == 0 && y == 0)
					return;
				
				//Check each neighbouring cell and push it to the stack if possible
				if(x < 15 && getE(temp) == 0 && getDist(mazecells[x+1][y]) == 255
					&& !getW(mazecells[x+1][y]) )//Check the next cell too as it may contain wall data
				{
					push(mazecells[x+1][y], &stack2);
				}
				if(x > 0 && getW(temp) == 0 && getDist(mazecells[x-1][y]) == 255
					&& !getE(mazecells[x-1][y]) )
				{
					push(mazecells[x-1][y], &stack2);
				}
				if(y < 15 && getS(temp) == 0 && getDist(mazecells[x][y+1]) == 255
					&& !getN(mazecells[x][y+1]) )
				{
					push(mazecells[x][y+1], &stack2);
				}
				if(y > 0 && getN(temp) == 0 && getDist(mazecells[x][y-1]) == 255
					&& !getS(mazecells[x][y-1]) )
				{
					push(mazecells[x][y-1], &stack2);
				}
			}	
			//Remove From Stack
			popStack(&stack1);	
		}
		
		//Increment what level we are checking
		level++;
		
		while(!stackIsEmpty(&stack2))
		{	
			long tp = top(&stack2);
			popStack(&stack2);
			
			push(tp, &stack1);
		}
	}
}

//Reset Maze to 0
void initializeMaze(long cell[16][16])
{
	for(int i=0; i < 16;i++)
	{
		for(int j=0; j < 16; j++)
		{
			cell[i][j]=0;
			setXY(&cell[i][j],i,j); //set x,y coordinates of each cell
			setDistance(&cell[i][j],255); //sets initial value of cells to 0
			setExp(&cell[i][j],0); //sets initial value of cells to 0
			setN(&cell[i][j],0); //sets Nwall bit of cells to 0
			setE(&cell[i][j],0); //sets Ewall bit of cells to 0
			setS(&cell[i][j],0); //sets Swall bit of cells to 0
			setW(&cell[i][j],0); //sets Wwall bit of cells to 0
		}
	}
}