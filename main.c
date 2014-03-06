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
#include <math.h>

//Mouse Profile
#include "RobotModel/RobotModel.h"
#include "RobotModel/RobotModel_Controls.h"
#include "RobotModel/RobotModel_StepperMotor.h"
#include "RobotModel/RobotModel_Sensors.h"

//Debugging
#include "USART.h"

volatile unsigned long milliseconds;
extern volatile Mouse mouse;
void startTimer(void);

int main(void)
{
	setupADC();	
	setupStepperMotor();
	startTimer();

	USART_init();
	
	mouse.velocity = 0;
	mouse.maxVelocity = 5000;
	mouse.acceleration = 2000;
	mouse.deceleration = 10000;

	enableDrive(1);
	turnOnTimers(1,1);
	
	/*for(int i = 0; i < 20; i++)
	{		
		int right = isWallRight();
		int front = isWallFront();
		int left = isWallLeft();
		
		if(!right)
		{
			rotateRight();
		}
		else if(front && !left)
		{
			rotateLeft();
		}
		else if(front)
		{
			moveBackwardsAndCorrect();
		}	
		
		if(left && right)		
			mouse.IR_CORRECT = 20;
		
		moveForwardAndStop();
		
		mouse.IR_CORRECT = 0;
		
		
	}*/

	turnOnTimers(0, 0);
	enableDrive(0);

	while(1==1)
	{
		//Testing Pot, Button and led's on front panel
		//float but = isButtonPushed(1);
		//float pot = getPotSensorValue(0);
		//int num = pot / 129;
		
		//if(but)
		//	turnOnLeds(num);
		//else
		//	turnOnLeds(0);
		
		printlnNum(getPotSensorValue(7));
		

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
