#include <avr/io.h>
#include <util/delay.h>

#include "RobotModel.h"
#include "RobotModel_Controls.h" 
#include "RobotModel_Sensors.h"

#include "../USART.h"
extern volatile long milliseconds;
extern volatile Mouse mouse;


void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{
	unsigned long startTime = milliseconds;
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	
	while(inSpeed + accel*(float)((milliseconds-startTime)/1000.0) < maxSpeed)
	{
		float curSpeed = inSpeed + accel*(float)((milliseconds-startTime)/1000.0);
		
		int error = getOffsetError();
		
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(curSpeed - error);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(curSpeed + error);
		
		float decelStepsUntilStop = curSpeed * curSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
		
		//If we need to start deceleratings
		if(mouse.leftMotor.stepCount + decelStepsUntilStop >= stepTarget)
		{
			maxSpeed = curSpeed;
			break;
		}
		
		//If we're accelerating past our stepTarget;
		if(mouse.leftMotor.stepCount >= stepTarget)
		{
			return;
		}
		_delay_ms(1);
	}	
	
	//Calculate when to start decelerating
	float decelSteps = (float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);

	//Decelerate n stuff
	while(mouse.leftMotor.stepCount + decelSteps < stepTarget);
	
	startTime = milliseconds;
		
	while(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0) > exitSpeed)
	{
		float curSpeed = maxSpeed - decel*(float)((milliseconds-startTime)/1000.0);
		
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay =  getDelayFromVelocity(curSpeed);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(curSpeed);
		
		_delay_ms(1);
	}

}

volatile float lPrevious;
volatile float rPrevious;
volatile int flag;

int getOffsetError()
{
	if(flag != 1)
	{
		lPrevious = getLeftIR();
		rPrevious = getRightIR();
		flag = 1;
	}
	
	float left = getLeftIR();
	float right = getRightIR();
			
	//If derivative of IR readings is greater then 1
	if(left-lPrevious > 1)
	{		
		mouse.IR_CORRECT = 0;
	}
	if(right-rPrevious > 1)
	{
		mouse.IR_CORRECT = 0;
	}
	
	float error = (right-left)*mouse.IR_CORRECT;
	
	if(right-left > 3)
		error = 3*mouse.IR_CORRECT;
	if(right-left < -3)
		error = -3*mouse.IR_CORRECT;

		
	lPrevious = left;
	rPrevious = right;
	return error;
}
