 /*
 * Stepper Motor Controller - For Trinity Firefighting Robot and Micromouse	
 *
 * @author Justin Marple, Eli Siskind
 * @version Feb 3rd 2014
 *
 */

 //Avr Includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

//Robot Profile
#include "RobotModel_StepperMotor.h"
#include "RobotModel.h"

//Mouse variable
extern volatile Mouse mouse;

/* LEFT MOTOR CONTROL */
ISR(TIMER1_COMPA_vect)
{
	//Pulse Motor
	pulseMotor(&PORTD, PD1);
	
	//Update Counters
	mouse.leftMotor.stepCount++;		
	mouse.leftMotor.totalCount++;
	
	//Update Timer
	OCR1A = mouse.leftMotor.currentStepDelay;
}

/* RIGHT MOTOR CONTROL */
ISR(TIMER3_COMPA_vect)
{	
	//Pulse motor
	pulseMotor(&PORTB, PB7);
	
	//Update Counters
	mouse.rightMotor.stepCount++;
	mouse.rightMotor.totalCount++;
	
	//Update Timer
	OCR3A = mouse.rightMotor.currentStepDelay;
}

//Setup Registers and timers to start controlling the motors
void setupStepperMotor()
{
	//Motor Pins are D2 and D4 on arduino nano
	DDRD |= (1 << PORTD1) | (1 << PORTD2) | (1 << PORTD3);
	DDRB |= (1 << PORTB1) | (1 << PORTB0) | (1 << PORTB6) | (1 << PORTB7);
	
	//Set to CTC
	TCCR1B = (1 << WGM12);
	TCCR3B = (1 << WGM32);
	
	//Compare Ticks
	OCR1A = 0;
	OCR3A = 0;
	
	//Enable Interrupts
	TIMSK1 = (1 << OCIE1A);
	TIMSK3 = (1 << OCIE3A);
	
	//Set Prescalar to 0
	OCR1B = 0;
	OCR3B = 0;
	
	//Enable Interrupts
	sei(); 
	
	enableDrive(0);	
}

//Turn on/off the drive in hardware
void enableDrive(int left)
{
	if(left)
		PORTB &= ~(1 << PB6);
	else
		PORTB |= (1 << PB6);
}

/* Pulse port and pin for 10 microseconds */
void pulseMotor(volatile uint8_t *port, uint8_t pin)
{
	//Send Pulse to Motor
	(*port) |= (1 << pin);
	_delay_us(5);
	(*port) &= ~(1 << pin);
}

//Set Direction of motors in hardware
void setDirection(int left, int right)
{
	if(left)
		PORTD &= ~(1 << PD0);
	else
		PORTD |= (1 << PD0);
		
	if(right)
		PORTD |= (1 << PD6);
	else
		PORTD &= ~(1 << PD6);
}

//Turn timers on for controlling motors
void turnOnTimers(int one, int two)
{
	if(one)
		TCCR1B |= (1 << CS11) | (1 << CS10);
	else
		TCCR1B &= ~((1 << CS11) | (1 << CS10));
		
	if(two)
		TCCR3B |= (1 << CS31) | (1 << CS30);
	else	
		TCCR3B &= ~((1 << CS31) | (1 << CS30));
}

//Convert steps per second to counter pulses per steps
int getDelayFromVelocity(int stepsPerSecond)
{
	if(stepsPerSecond < 10)
		return 25000;
	else
		return TIMER_FREQUENCY / stepsPerSecond;
}
