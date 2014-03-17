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
Last Updated: March 15th 2014

**********************************************/

//Everything is controlled via mouse manager
#include "MouseManager/mouseManager.h"

//Start Point of our program
int main(void)
{
	//Setup mouse timers and functions
	initializeMouse();
	
	//Solve our maze
	solveMaze();
	
	//Stop all timers and functions
	stopMouse();	
	
	while(1==1){}	
}