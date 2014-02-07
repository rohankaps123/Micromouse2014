/*
 * Stepper Motor Controller - For Trinity Firefighting Robot and Micromouse	
 *
 * @author Justin Marple, Eli Siskind
 * @version Feb 3rd 2014
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "StepperMotor.h"



StepperMotor leftMotor;
StepperMotor rightMotor;
extern long unsigned microseconds;



/* LEFT MOTOR CONTROL */
ISR(TIMER0_COMPA_vect)
{
	//Set to max initially to prevent the next interrupt cycle from overwriting this one
	OCR0A = 255;
	
	//If we are ready to step the motor again
	if(leftMotor.delayCounter == 0)
	{
		//Pulse and increment motor
		pulseMotor(&PORTD, PD2);
		leftMotor.stepCount++;		
		
		//Calculate the next delay
		leftMotor.delayCounter = calculateNextDelay(&leftMotor, &TCCR0B);
	}

	//Set Interrupt Timer, since somtimes we need to wait a value larger then 255, modify delayCounter accordingly
	OCR0A = eightBitTimerFix(&leftMotor);	
}



/* RIGHT MOTOR CONTROL */
ISR(TIMER2_COMPA_vect)
{
	//Set to max initially to prevent the next interrupt cycle from overwriting this one
	OCR2A = 255;
	
	//If we are ready to step the motor again
	if(rightMotor.delayCounter == 0)
	{
		//Pulse and increment motor
		pulseMotor(&PORTD, PD4);
		rightMotor.stepCount++;
		
		//Calculate the next delay
		rightMotor.delayCounter = calculateNextDelay(&rightMotor, &TCCR2B);	
	}
	
	//Set Interrupt Timer, since somtimes we need to wait a value larger then 255, modify delayCounter accordingly
	OCR2A = eightBitTimerFix(&rightMotor);		
}

void accelerateMotor(struct StepperMotor *motor, int continueTo)
{
	motor->accelCount++;
	
	//Calculate next interrupt delay
	motor->currentStepDelay -= (float)(2.0 * motor->currentStepDelay) / (float)(4.0 * motor->accelCount + 1);
	
	//If we're at our target speed
	if(motor->totalAccelSteps <= motor->accelCount)
	{
		motor->mode = continueTo;
		motor->currentStepDelay = motor->minDelay;
	}
}

void runMotor(struct StepperMotor *motor, int continueTo)
{
	//Reassign the interrupt delay as max speed 
	motor->currentStepDelay = motor->minDelay;
	
	//If we're ready to decelerate, switch to DECEL
	if(motor->stepCount >= motor->totalRunSteps)
	{
		motor->mode = continueTo;		
	}
}

void decelerateMotor(struct StepperMotor *motor, int continueTo)
{
	motor->decelCount--;
	motor->currentStepDelay += (float)(2.0 * motor->currentStepDelay) / (float)(4.0 * motor->decelCount + 1);
	
	//If we're at the stopping point
	if(motor->decelCount <= motor->decelStop)
	{
		motor->mode = continueTo;
	}
}

//Function that 
int calculateNextDelay(struct StepperMotor *motor, volatile uint8_t *shutoffPort)
{
	switch(motor->mode)
	{	
		//Accelerate to max Speed
		case ACCEL:			
			accelerateMotor(motor, RUN);				
			break;
		
		//Run at max speed
		case RUN:
			runMotor(motor, DECEL);
			break;
			
		//Decelerate to exit speed
		case DECEL:			
			decelerateMotor(motor, STOP);
			break;
			
		case STOP:
			//(*shutoffPort) = 0;//Shut off timer (prescalar set to 0 with CTC still enabled)
			//enableDrive(0, 0);
			break;
	}
	
	return motor->currentStepDelay;
}

void setupStepperMotor()
{
	//Motor Pins are D2 and D4 on arduino nano
	DDRD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4);
	DDRB |= (1 << PORTB1) | (1 << PORTB0);
	//Set to CTC
	TCCR0A = (1 << WGM01);
	TCCR2A = (1 << WGM21);
	
	//Compare Ticks
	OCR0A = 1;
	OCR2A = 1;
	
	//Enable Interrupts
	TIMSK0 = (1 << OCIE0A);
	TIMSK2 = (1 << OCIE2A);
	
	//Set Prescalar to 0
	OCR0B = 0;
	OCR2B = 0;
	
	//Enable Interrupts
	sei(); 
	
	enableDrive(0, 0);	
}

void enableDrive(int left, int right)
{
	if(left == 1)	
		PORTD &= ~(1 << PD6);	
	else if(right == 0)
		PORTD |= (1 << PD6);
	
	if(right == 1)	
		PORTD &= ~(1 << PD7);	
	else if(right == 0)	
		PORTD |= (1 << PD7);	
}

/* Pulse port and pin for 10 microseconds */
void pulseMotor(volatile uint8_t *port, uint8_t pin)
{
	//Send Pulse to Motor
	(*port) |= (1 << pin);
	_delay_us(10);
	(*port) &= ~(1 << pin);
}

void setDirection(int left, int right)
{
	if(left)
		PORTB &= ~(1 << PB0);
	else
		PORTB |= (1 << PB0);
		
	if(right)
		PORTB |= (1 << PB1);
	else
		PORTB &= ~(1 << PB1);
}


int eightBitTimerFix(struct StepperMotor *motor)
{
	int returnNum;
	
	if(motor->delayCounter <= 255)
	{
		returnNum = motor->delayCounter;
		motor->delayCounter = 0;	
	}
	else
	{
		motor->delayCounter -= 255;
		returnNum = 255;
	}
	
	return returnNum;
}

void turnOnTimers(int one, int two)
{
	if(one)
		TCCR0B |= (1 << CS01) | (1 << CS00);
		
	if(two)
		TCCR2B |= (1 << CS22);
}
