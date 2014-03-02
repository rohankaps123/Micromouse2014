#include <avr/io.h>
#include <util/delay.h>

#include "RobotModel.h"
#include "RobotModel_Controls.h" 

extern volatile long milliseconds;
extern volatile Mouse mouse;

void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{
	unsigned long startTime = milliseconds;
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	
	while(inSpeed + accel*(float)((milliseconds-startTime)/1000.0) < maxSpeed)
	{
		float curSpeed = inSpeed + accel*(float)((milliseconds-startTime)/1000.0);
		
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(curSpeed);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(curSpeed);
		float decelStepsUntilStop = curSpeed*curSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);
		
		//If we need to start decelerating
		if(mouse.leftMotor.stepCount + decelStepsUntilStop >= stepTarget)
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
	while(mouse.leftMotor.stepCount + decelSteps < stepTarget);
	
	startTime = milliseconds;
		
	while(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0) > exitSpeed)
	{
		mouse.leftMotor.currentStepDelay =  getDelayFromVelocity(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0));
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0));
		_delay_ms(1);
	}
}