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
#include "../USART.h"

//These will hold which cells to add distances to
struct llstack stack1;
struct llstack stack2;
	
//Refresh Distances in FloodFill Maze
void floodFill(long mazecells[16][16], char goal, int endX, int endY)
{
	//What is the current number?
	int level=0;
	int canGoInUnexploredCells = 1;
	
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
	else if(goal == 'S')
	{
		//Set our center to "0"
		canGoInUnexploredCells = 0;
		push(mazecells[7][7], &stack1);		
		push(mazecells[7][8], &stack1);		
		push(mazecells[8][7], &stack1);		
		push(mazecells[8][8], &stack1);
	}
	//If our goal is our testing area
	else if(goal == '6')
	{
		push(mazecells[5][5], &stack1);
	}
	else if(goal == '7')
	{
		push(mazecells[10][5], &stack1);
	}
	else if(goal == '8')
	{
		canGoInUnexploredCells = 0;
		push(mazecells[5][5], &stack1);
	}
	else if(goal == '9')
	{
		canGoInUnexploredCells = 0;
		push(mazecells[10][5], &stack1);
	}
	else if(goal == 'L')
	{
		//canGoInUnexploredCells = 0;
		push(mazecells[0][0], &stack1);
	}
	else if(goal == 'R')
	{
		//canGoInUnexploredCells = 0;
		push(mazecells[15][0], &stack1);
	}
	//Otherwise, default to start point
	else
	{
		push(mazecells[0][0], &stack1);
	}

	int lc = 0;
	while(!stackIsEmpty(&stack1))
	{
		//printMaze(mazecells);
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
				
			 	if(x == endX && y == endY)
					return;
				 
				//Check each neighbouring cell and push it to the stack if possible
				if(x < 15 && getE(temp) == 0 && getDist(mazecells[x+1][y]) == 255
					&& !getW(mazecells[x+1][y]) && (canGoInUnexploredCells || getExp(mazecells[x+1][y]) == 1) )//Check the next cell too as it may contain wall data
				{
					push(mazecells[x+1][y], &stack2);
				}
				if(x > 0 && getW(temp) == 0 && getDist(mazecells[x-1][y]) == 255
					&& !getE(mazecells[x-1][y]) && (canGoInUnexploredCells || getExp(mazecells[x-1][y]) == 1))
				{
					push(mazecells[x-1][y], &stack2);
				}
				if(y < 15 && getS(temp) == 0 && getDist(mazecells[x][y+1]) == 255
					&& !getN(mazecells[x][y+1]) && (canGoInUnexploredCells || getExp(mazecells[x][y+1]) == 1))
				{
					push(mazecells[x][y+1], &stack2);
				}
				if(y > 0 && getN(temp) == 0 && getDist(mazecells[x][y-1]) == 255
					&& !getS(mazecells[x][y-1]) && (canGoInUnexploredCells || getExp(mazecells[x][y-1]) == 1))
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
		
		//Error in Code
		lc++;
		if(lc > 255)
		{
			//Turn OFF Timers
			TCCR1B &= ~((1 << CS11) | (1 << CS10));
			TCCR3B &= ~((1 << CS31) | (1 << CS30));
			
			//Blink LED's, signalling something went wrong.  
			while(!isButtonPushed(1))
			{
				turnOnLeds(7);
				_delay_ms(20);
				turnOnLeds(0);
				_delay_ms(20);
			}
			
			//Print Data when button is pushed
			printMaze(mazecells);
			while(1==1);
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
// set the outer walls
	for(int i=0; i < 16;i++){
		setN(&cell[i][0],1);
		setS(&cell[i][15],1);
		setW(&cell[0][i],1);
		setE(&cell[15][i],1);
		}
}