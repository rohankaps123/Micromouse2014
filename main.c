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
#include <avr/interrupt.h>

//Mouse Profile
#include "RobotModel/RobotModel.h"
#include "RobotModel/RobotModel_Controls.h"
#include "RobotModel/RobotModel_StepperMotor.h"

//Debugging
#include "USART.h"

volatile unsigned long milliseconds;
void startTimer(void);

int main(void)
{
	setupADC();	
	setupStepperMotor();
	startTimer();


	USART_init();

	setDirection(0,0);
	enableDrive(1);
	turnOnTimers(1,1);
	
	//straight(3000, 0, 5000, 0, 3000, 12000);
	
	turnOnTimers(0, 0);
	enableDrive(0);
	while(1==1)
	{
	}	
}

void startTimer()
{
	//Refresh Loop Timer1  
	TCCR0A = (1 << WGM01);//Set CTC
	TCCR0B = (1 << CS00) | (1 << CS01);//prescalar to
	OCR0A = 250;//Compare Ticks 
    TIMSK0 = (1 << OCIE0A);//Enable Timer Interrupts
	
	milliseconds = 0;
}

ISR(TIMER0_COMPA_vect)
{
	milliseconds++;	
}
