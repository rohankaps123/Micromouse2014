#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "StepperMotor.h"
#include "StepperControl.h"
#include "USART.h"

extern StepperMotor leftMotor;
extern StepperMotor rightMotor;
volatile unsigned long milliseconds;
void startTimer(void);

int main(void)
{

	setupStepperMotor();
	startTimer();
	
	USART_init();
	
	turnOnTimers(1,1);
	setDirection(0, 0);
	enableDrive(1);
	
	straight(8000, 0, 4000, 0, 4000, 4000);
	
	turnOnTimers(0, 0);
	enableDrive(0);
	
	while(1==1){}	
}

void startTimer()
{
	//Refresh Loop Timer1  
	TCCR0A = (1 << WGM01);//Set CTC
	TCCR0B = (1 << CS00) | (1 << CS01);//prescalar to
	OCR0A = 125;//Compare Ticks 
    TIMSK0 = (1 << OCIE0A);//Enable Timer Interrupts
	
	milliseconds = 0;
}

ISR(TIMER0_COMPA_vect)
{
	milliseconds++;	
}
