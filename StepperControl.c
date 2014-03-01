#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "StepperMotor.h"
#include "StepperControl.h"
#include "Sensors.h"

extern StepperMotor leftMotor;
extern StepperMotor rightMotor;
extern volatile unsigned long milliseconds;

void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{	
	unsigned long startTime = milliseconds;
	rightMotor.stepCount = leftMotor.stepCount = 0;
	
	
	while(inSpeed + accel*(float)((milliseconds-startTime)/1000.0) < maxSpeed)
	{
		float curSpeed = inSpeed + accel*(float)((milliseconds-startTime)/1000.0);
		
		leftMotor.currentStepDelay = getDelayFromVelocity(curSpeed);
		rightMotor.currentStepDelay = getDelayFromVelocity(curSpeed);
		float decelStepsUntilStop = curSpeed*curSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
		
		//If we need to start decelerating
		if(leftMotor.stepCount + decelStepsUntilStop >= stepTarget)
		{
			maxSpeed = curSpeed;
			break;
		}
		getIRSensorValue(&PORTD, PD4, 0);
		_delay_ms(1);
	}	
	
	//Calculate when to start decelerating
	float decelSteps = (float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);

	//Decelerate n stuff
	while(leftMotor.stepCount + decelSteps < stepTarget);
	
	startTime = milliseconds;
		
	while(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0) > exitSpeed)
	{
		leftMotor.currentStepDelay =  getDelayFromVelocity(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0));
		rightMotor.currentStepDelay = getDelayFromVelocity(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0));
		_delay_ms(1);
	}
	
}

void inPlaceTurn(int degrees, int maxSpeed, int accel, int decel)
{
	int turningConstant = 10;
	
	setDirection(0, 1);
	straight(degrees * turningConstant, 0, maxSpeed, 0, accel, decel);
}
