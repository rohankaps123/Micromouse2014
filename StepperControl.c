#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "StepperMotor.h"
#include "StepperControl.h"

extern StepperMotor leftMotor;
extern StepperMotor rightMotor;

void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{
	
	//Motor settings
	leftMotor.mode 			= rightMotor.mode 		= ACCEL;
	leftMotor.stepCount 	= rightMotor.stepCount	= 0;
	
	//Motor's max speed converted to a delay
	leftMotor.minDelay		= rightMotor.minDelay 	= TIMER_FREQUENCY / maxSpeed;
	
	//Current STep Delay
	leftMotor.currentStepDelay = rightMotor.currentStepDelay = calculateInitialDelay(accel);	
	
	//Calculate the number of steps needed for total deceleration
	leftMotor.decelStop 	= rightMotor.decelStop  	= (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
		
	//Calculate current step delay for the start of acceleration
	int accelStepsToStart = (float)inSpeed * (float)inSpeed / (2.0 * (float)accel);
	int tempAccelCount = 0;
	float tempCurrentStepDelay = leftMotor.currentStepDelay;
	
	for(int i = 0; i < accelStepsToStart; i++)
	{
		tempAccelCount++;
		tempCurrentStepDelay -= (float)(2.0 * tempCurrentStepDelay) / (float)(4.0 * tempAccelCount + 1);
	}
	
	//Set Starting Accel Count and Decel Counts
	rightMotor.accelCount 			= leftMotor.accelCount 		= tempAccelCount;
	leftMotor.decelCount 			= rightMotor.decelCount 	= (float)maxSpeed * (float)maxSpeed / (2.0 * decel);
	
	//Updating starting delay
	rightMotor.currentStepDelay 	= leftMotor.currentStepDelay 	= tempCurrentStepDelay;

	//Calculate the total number of steps for each action
	leftMotor.totalAccelSteps =		rightMotor.totalAccelSteps 	= (float)maxSpeed * (float)maxSpeed / (2.0 * accel);
	leftMotor.totalDecelSteps = 	rightMotor.totalDecelSteps 	= (float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
	leftMotor.totalRunSteps   = 	rightMotor.totalRunSteps 	= stepTarget - leftMotor.totalDecelSteps;	
	
	//Reset Delay Counter for 8-bit fix
	leftMotor.delayCounter 	= rightMotor.delayCounter 	= 0;

	turnOnTimers(1,1);	
	enableDrive(1, 1);
	while(leftMotor.mode != STOP);
}

void inPlaceTurn()
{
}

void turn(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{
	//Motor settings
	leftMotor.mode 			= ACCEL;
	leftMotor.stepCount 	= 0;
	
	//Motor's max speed converted to a delay
	leftMotor.minDelay		= TIMER_FREQUENCY / maxSpeed;
	
	//Current STep Delay
	leftMotor.currentStepDelay = calculateInitialDelay(accel);	
	
	//Calculate the number of steps needed for total deceleration
	leftMotor.decelStop 	= (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
		
	//Calculate current step delay for the start of acceleration
	int accelStepsToStart = (float)inSpeed * (float)inSpeed / (2.0 * (float)accel);
	int tempAccelCount = 0;
	float tempCurrentStepDelay = leftMotor.currentStepDelay;
	
	for(int i = 0; i < accelStepsToStart; i++)
	{
		tempAccelCount++;
		tempCurrentStepDelay -= (float)(2.0 * tempCurrentStepDelay) / (float)(4.0 * tempAccelCount + 1);
	}
	
	//Set Starting Accel Count and Decel Counts
	leftMotor.accelCount 	= tempAccelCount;
	leftMotor.decelCount 	= (float)maxSpeed * (float)maxSpeed / (2.0 * decel);
	
	//Updating starting delay
	leftMotor.currentStepDelay 	= tempCurrentStepDelay;

	//Calculate the total number of steps for each action
	leftMotor.totalAccelSteps = (float)maxSpeed * (float)maxSpeed / (2.0 * accel);
	leftMotor.totalDecelSteps = (float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
	leftMotor.totalRunSteps   = stepTarget - leftMotor.totalDecelSteps;	
	
	//Reset Delay Counter for 8-bit fix
	leftMotor.delayCounter 	= 0;

	turnOnTimers(1,0);	
	enableDrive(0, 0);
	
	while(leftMotor.mode != STOP);
}

float calculateInitialDelay(float accel)
{
	return 0.676 * TIMER_FREQUENCY * sqrt(2.0 / (float)accel);	
}

