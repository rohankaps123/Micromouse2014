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

void smoothTurn(float degrees, float midRadius, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel)
{
	setDirection(0, 0);
	
	float smallArc = TURN_CONST * degrees * (midRadius - MOUSE_WIDTH_HALF);
	float leadArc =  TURN_CONST * degrees * midRadius;
	float largeArc = TURN_CONST * degrees * (midRadius + MOUSE_WIDTH_HALF);
	
	float smallRatio = smallArc / leadArc;
	float largeRatio = largeArc / leadArc;

	unsigned long startTime = milliseconds;
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;	
	
	while(inSpeed + accel*(float)((milliseconds-startTime)/1000.0) < maxSpeed)
	{
		float curSpeed = inSpeed + accel*(float)((milliseconds-startTime)/1000.0);
		
		int leftSpeed = curSpeed * smallRatio;
		int rightSpeed = curSpeed * largeRatio;	
		
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(leftSpeed);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(rightSpeed);
		
		float stepsForDeceleration = curSpeed * curSpeed / (2 * decel);
		float stepsAlready = curSpeed * curSpeed / (2 * accel);
		
		if(stepsAlready + stepsForDeceleration >= leadArc)
		{
	 		maxSpeed = curSpeed;
			printlnNum((float)mouse.leftMotor.stepCount / smallRatio);
			printlnNum((float)stepsAlready);
			printlnNum((float)stepsForDeceleration);
			printlnNum((float)leadArc);
			break;
		}
	}
	
	startTime = milliseconds;
		
	while(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0) > exitSpeed)
	{		
		float curSpeed = maxSpeed - decel*(float)((milliseconds-startTime)/1000.0);	
		
		//Our left and right velocities are proportional to the arc lengths of each path
		int leftSpeed = curSpeed * smallRatio;
		int rightSpeed = curSpeed * largeRatio;			
		
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay =  getDelayFromVelocity(leftSpeed);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(rightSpeed);
		
		_delay_ms(1);
	}
	
	printlnNum((float)mouse.leftMotor.stepCount);
	printlnNum((float)smallArc);
	printlnNum((float)smallRatio);
	//Mouse Constants (should be defined somewhere)
	/*int wheelCircum = 18.8495;
	int stepsPerRotation = 800;
	int mouseWidth = 10;
	
	//Calculate how far the middle of the mouse will be traveling in steps
	float arclength_mid = (2 * M_PI * midRadius * (degrees / 360.0)) / wheelCircum * stepsPerRotation;
	
	//Calculate the distance of the two wheels
	float arclength_high = (2 * M_PI * (midRadius + (mouseWidth / 2.0)) * (degrees / 360.0)) / wheelCircum * stepsPerRotation;
	float arclength_low = (2 * M_PI * (midRadius - (mouseWidth / 2.0)) * (degrees / 360.0)) / wheelCircum * stepsPerRotation; 

	float arclength_left;
	float arclength_right;
	float ratioLeft;
	float ratioRight;
	
	if(degrees > 0)
	{
		arclength_left = arclength_high;
		arclength_right = arclength_low;
		ratioLeft = arclength_high / arclength_mid;
		ratioRight = arclength_low / arclength_mid;
	}
	else if(degrees < 0)
	{
		arclength_mid *= -1;
		arclength_high *= -1;
		arclength_low *= -1;
		degrees *= -1;
		
		arclength_left = arclength_low;
		arclength_right = arclength_high;		
		ratioLeft = arclength_low / arclength_mid;
		ratioRight = arclength_high / arclength_mid;
	}
	else
	{ 
		return;
	}
	
	printlnNum(ratioLeft);
	printlnNum(ratioRight);
	printlnNum(arclength_left);
	printlnNum(arclength_right);
	printlnNum((midRadius - (mouseWidth / 2.0)));
	
	//Reset Mouse Constants
	unsigned long startTime = milliseconds;
	mouse.rightMotor.stepCount = mouse.leftMotor.stepCount = 0;			
	
	while(inSpeed + accel*(float)((milliseconds-startTime)/1000.0) < maxSpeed)
	{
		float curSpeed = inSpeed + accel*(float)((milliseconds-startTime)/1000.0);
		int leftSpeed = 0;
		int rightSpeed = 0;		
		
		//Our left and right velocities are proportional to the arc lengths of each path
		leftSpeed = curSpeed * ratioLeft;
		rightSpeed = curSpeed * ratioRight;		
		
		//Update motors
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay = getDelayFromVelocity(leftSpeed);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(rightSpeed);
			
		float decelSteps_left = ( curSpeed * curSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel) ) * ratioLeft;
		
		//If we need to start deceleratings
		if(mouse.leftMotor.stepCount + decelSteps_left >= arclength_left)
		{ 
			maxSpeed = curSpeed;
			print("Break\n\r");
			printlnNum((float)curSpeed);
			printlnNum((float)mouse.leftMotor.stepCount);
			printlnNum((float)mouse.leftMotor.stepCount / ratioLeft);
			printlnNum((float)decelSteps_left);
			printlnNum((float)arclength_mid);
			break;
		}
		
		_delay_ms(1);	
	}
	
	
	//Calculate when to start decelerating
	float decelSteps =  ((float)maxSpeed * (float)maxSpeed / (2.0 * decel) - (float)exitSpeed * (float)exitSpeed / (2.0 * decel)) *ratioLeft;

	//Decelerate n stuff
	while(mouse.leftMotor.stepCount + decelSteps < arclength_left);
		
		
	startTime = milliseconds;
	
	while(maxSpeed - decel*(float)((milliseconds-startTime)/1000.0) > exitSpeed)
	{		
		float curSpeed = maxSpeed - decel*(float)((milliseconds-startTime)/1000.0);
		
		int leftSpeed = 0;
		int rightSpeed = 0;		
		
		//Our left and right velocities are proportional to the arc lengths of each path
		leftSpeed = curSpeed * ratioLeft;
		rightSpeed = curSpeed * ratioRight;			
		
		mouse.velocity = curSpeed;
		mouse.leftMotor.currentStepDelay =  getDelayFromVelocity(leftSpeed);
		mouse.rightMotor.currentStepDelay = getDelayFromVelocity(rightSpeed);
		
		_delay_ms(1);
	}

	printlnNum(mouse.leftMotor.stepCount);*/
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
		mouse.IR_CORRECT_LEFT = 0;
		mouse.IR_CORRECT_RIGHT = 0;
	}
	if(right-rPrevious > 1)
	{
		mouse.IR_CORRECT = 0;
		mouse.IR_CORRECT_LEFT = 0;
		mouse.IR_CORRECT_RIGHT = 0;
	}
	
	float error = 0;
	
	if(mouse.IR_CORRECT != 0)
	{
		error = (right-5)*mouse.IR_CORRECT;
	
		if(right-left > 10)
			error = 3*mouse.IR_CORRECT;
		if(right-left < -10)
			error = -3*mouse.IR_CORRECT;
	}
	else if(mouse.IR_CORRECT_LEFT != 0)
	{
		error = getLeftError() * mouse.IR_CORRECT_LEFT;
	}
	else if(mouse.IR_CORRECT_RIGHT != 0)
	{
		error = getRightError() * mouse.IR_CORRECT_RIGHT;
	}
		
	lPrevious = left;
	rPrevious = right;
	
	return error;
}

float getRightError()
{
	float right = getRightIR();
	return right - 5;
}

float getLeftError()
{
	float left = getLeftIR();
	return left - 5;
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
