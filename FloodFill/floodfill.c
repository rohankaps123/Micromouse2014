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

#include "floodfill.h"
#include "DataComponents.h"
#include "Stack.h"

//Debugging
#include "USART.h"

//Refresh Distances in FloodFill Maze
void floodFill(long mazecells[16][16], char goal)
{
	//What is the current number?
	int level=0;
	
	//These will hold which cells to add distances to
	struct llstack stack1;
	struct llstack stack2;
	
	//Reset Stack to null
	init_stack(&stack1);
	init_stack(&stack2);
	
	//If our goal is the center..
	if(goal == 'C')
	{
		//Set our center to "0"
		setDistance(&mazecells[7][7], 0);
		push(mazecells[7][7], &stack1);
		
		setDistance(&mazecells[7][8], 0);
		push(mazecells[7][8], &stack1);
		
		setDistance(&mazecells[8][7], 0);
		push(mazecells[8][7], &stack1);
		
		setDistance(&mazecells[8][8], 0);
		push(mazecells[8][8], &stack1);
	}
	//Otherwise, default to start point
	else
	{
		setDistance(&mazecells[0][0], 0);
		push(mazecells[0][0], &stack1);
	}
	
	//while(!stackIsEmpty(&stack1))
	//{
		//Get value of  
		//long temp = top(&stack1);
		//setDistance(temp);
		//pop(&stack1);
	//}	
	
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

//void solveMaze()
//{
	/*initialize_maze();
	
	setDestination(Destination)
	while(getValue(currCell)!=0)
	{
		check_walls_update();
		floodfill(cells,'C');
	}*/
//}
