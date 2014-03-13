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

//Mouse Profile
#include "RobotModel/RobotModel.h"
#include "RobotModel/RobotModel_Controls.h"
#include "RobotModel/RobotModel_StepperMotor.h"
#include "RobotModel/RobotModel_Sensors.h"

//Debugging
#include "USART.h"

int currCell=0;
extern int cells[MAZEHEIGHT][MAZEWIDTH];
 
void initialize_maze(){
	for(int i=0; i< MAZEHEIGHT;i++)
		for(int j=0;j<MAZEWIDTH;j++){
			cells[i][j]=0;
			setxy(cell[i][j],i,j); //set x,y coordinates of each cell
			setValue(cell[i][j],255); //sets initial value of cells to 0
			setExplored(cell[i][j],0); //sets initial value of cells to 0
			setNwall(cell[i][j],0); //sets Nwall bit of cells to 0
			setEwall(cell[i][j],0); //sets Ewall bit of cells to 0
			setSwall(cell[i][j],0); //sets Swall bit of cells to 0
			setWwall(cell[i][j],0); //sets Wwall bit of cells to 0
		}
}

void solveMaze()
{
	initialize_maze();
	
	setDestination(Destination)
	while(getValue(currCell)!=0)
	{
		check_walls_update();
		floodfill(cells,'C');
	}
}
