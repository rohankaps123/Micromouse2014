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
Last Updated: March 13th 2014

**********************************************/

//Basic Avr-gcc includes
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "mouseManager.h"

//Debugging
#include "USART.h"

int main(void)
{
	initializeMouse();
	
	solveMaze();
	
	stopMouse();	
	
	while(1==1){}	
}