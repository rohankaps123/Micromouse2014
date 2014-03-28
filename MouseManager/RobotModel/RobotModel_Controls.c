#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "RobotModel.h"
#include "RobotModel_Controls.h" 
#include "RobotModel_Sensors.h"

#include "../USART.h"
volatile long milliseconds;
extern volatile Mouse mouse;


void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{
	unsigned long startTime = milliseconds;
	//mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	
	while(inSpeed + accel*(float)((milliseconds-startTime)/1000.0) < maxSpeed)
	{
		updateSensors();
		
		float curSpeed = inSpeed + accel*(float)((milliseconds-startTime)/1000.0);
		
		float error = getOffsetError();
		
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
	}	
	
	//Calculate when to start decelerating
	float decelSteps = (float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel);

	//Decelerate n stuff
	while(mouse.leftMotor.stepCount + decelSteps < stepTarget)
	{
		updateSensors();
		
		float curSpeed = maxSpeed;
		
		float error = getOffsetError();
		
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(curSpeed - error);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(curSpeed + error);
	}
	
	startTime = milliseconds;
		
	while(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0) > exitSpeed)
	{	
		updateSensors();
		
		float curSpeed = maxSpeed - decel*(float)((milliseconds-startTime)/1000.0);
		
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay =  getDelayFromVelocity(curSpeed);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(curSpeed);
	}

}

void smoothTurn(float degrees, float midRadius, int inSpeed, float maxSpeed, int exitSpeed, int accel, int decel)
{
	setDirection(0, 0);
	
	int RIGHT = 1;
	if(degrees < 0)
	{
		RIGHT = 0;
		degrees = -degrees;
	}
	
	float smallArc = TURN_CONST * degrees * (midRadius - MOUSE_WIDTH_HALF);
	float leadArc =  TURN_CONST * degrees * midRadius;
	float largeArc = TURN_CONST * degrees * (midRadius + MOUSE_WIDTH_HALF);
	
	float smallRatio = smallArc / leadArc;
	float largeRatio = largeArc / leadArc;

	float smallSpeed = inSpeed * smallRatio;
	float largeSpeed = inSpeed * largeRatio;	
	
	unsigned long startTime = milliseconds;
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
		
	//Accelerate to speed difference
 	while(1 == 1)
	{
		float difference = accel*(float)((milliseconds+(TCNT0/16000.0)-startTime)/1000.0);
		
		int leftSpeed = inSpeed - difference;
		int rightSpeed = inSpeed + difference;	
		
		mouse.velocity = inSpeed;
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(leftSpeed * RIGHT + rightSpeed * (1-RIGHT));
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(rightSpeed * RIGHT + leftSpeed * (1-RIGHT));
		
		if(leftSpeed <= smallSpeed)
			break;
			
		int decelStepsLarge = (rightSpeed - inSpeed) * (rightSpeed - inSpeed) / (2 * decel);
		
		if(mouse.rightMotor.stepCount*RIGHT + mouse.leftMotor.stepCount*(1-RIGHT) + decelStepsLarge >= largeArc)
		{
			smallSpeed = leftSpeed;
			largeSpeed = rightSpeed;
			break;
		}	
	} 
	
	int decelStepsLarge = (largeSpeed - inSpeed) * (largeSpeed - inSpeed) / (2 * decel);
	
 	while(1==1)
	{
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(smallSpeed*RIGHT + largeSpeed*(1-RIGHT));
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(largeSpeed*RIGHT + smallSpeed*(1-RIGHT));
		
		if(mouse.rightMotor.stepCount*RIGHT + mouse.leftMotor.stepCount*(1-RIGHT) + decelStepsLarge >= largeArc)
			break;
	} 
	
 	startTime = milliseconds;
	while(1 == 1)
	{
		float difference = decel*(float)((milliseconds+(TCNT0/16000.0)-startTime)/1000.0);
		
		int leftSpeed = smallSpeed + difference;
		int rightSpeed = largeSpeed - difference;	
		
		mouse.velocity = inSpeed;
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(leftSpeed*RIGHT+rightSpeed*(1-RIGHT));
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(rightSpeed*RIGHT+leftSpeed*(1-RIGHT));
		
		if(leftSpeed >= inSpeed)
			break;
	}
}

volatile int flag;

float getOffsetError()
{

	if(flag != 1)
	{
		updateSensors();
		flag = 1;
	}
	
	float left = mouse.sensor[LEFT_IR].value;
	float right = mouse.sensor[RIGHT_IR].value;
	float lPrevious = mouse.sensor[LEFT_IR].previousValue;
	float rPrevious = mouse.sensor[RIGHT_IR].previousValue;
	
	//If derivative of IR readings is greater then 1
 	if(left-lPrevious > 0.5 || left-lPrevious < -0.5)
	{		
		mouse.IR_CORRECT = 0;
		mouse.IR_CORRECT_LEFT = 0;
		
		if(mouse.IR_LONG_CHECK_LEFT == 1)
		{
			mouse.IR_LONG_OFF_DISTANCE_LEFT = mouse.leftMotor.totalCount;
			mouse.IR_LONG_CHECK_LEFT = 0;
		}
	}
	if(right-rPrevious > 0.5 || right-rPrevious < -0.5)
	{
		mouse.IR_CORRECT = 0;
		mouse.IR_CORRECT_RIGHT = 0;
		
		if(mouse.IR_LONG_CHECK_RIGHT == 1)
		{
			mouse.IR_LONG_OFF_DISTANCE_RIGHT = mouse.rightMotor.totalCount;
			mouse.IR_LONG_CHECK_RIGHT = 0;
		}
	} 
	
	float error = 0;
	
	if(mouse.IR_CORRECT != 0)
	{
		error = (right-5)*mouse.IR_CORRECT;
	
		if(right-left > 5)
			error = 3*mouse.IR_CORRECT;
		if(right-left < -5)
			error = -3*mouse.IR_CORRECT;
	}
	else if(mouse.IR_CORRECT_LEFT != 0)
	{
		error =  - (left-5) * mouse.IR_CORRECT_LEFT;
	}
	else if(mouse.IR_CORRECT_RIGHT != 0)
	{
		error = (right-5) * mouse.IR_CORRECT_RIGHT;
		
		mouse.sensor[1].previousAverage = (mouse.sensor[1].previousValue - mouse.sensor[1].value + mouse.sensor[1].previousAverage*99)/100;
/* 		printNum(mouse.sensor[1].previousAverage);print(" => ");
		printNum((float)(mouse.sensor[1].previousValue - mouse.sensor[1].value));print(" => ");
		printlnNum((float)mouse.sensor[1].value); */
	}
	else
	{
	}
		
	// lPrevious = left;
	// rPrevious = right;
	return error;
}

float getRightError()
{
	float right = mouse.sensor[RIGHT_IR].value;
	return right - 5;
}

float getLeftError()
{
	float left = mouse.sensor[LEFT_IR].value;
	return left - 5;
}

void updateSensors()
{
	mouse.sensor[0].previousValue = mouse.sensor[0].value;
	mouse.sensor[1].previousValue = mouse.sensor[1].value;
	mouse.sensor[2].previousValue = mouse.sensor[2].value;
	mouse.sensor[3].previousValue = mouse.sensor[3].value;
	
	mouse.sensor[0].value = getLeftIR();
	mouse.sensor[1].value = getRightIR();
	mouse.sensor[2].value = getFrontLeftIR();
	mouse.sensor[3].value = getFrontRightIR();
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