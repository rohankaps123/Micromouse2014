/******************************
         RobotModel.c

 A model that controls the
 micromouse.  Includes IR and 
 stepper motor functions
 *****************************/

#include <avr/io.h>
#include <util/delay.h>

#include "RobotModel.h"   
#include "RobotModel_Controls.h" 

//Create Mouse
volatile Mouse mouse;

int isWallFront()
{
    float value = getFrontIR();
	
	//If there is something less then 16 cm away from sensor
	return (value < 16);		
}

int isWallRight()
{
	float value = getRightIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 8);
}

int isWallLeft()
{
	float value = getLeftIR();
	
	//If there is something less then 8 cm away from sensor 
	return (value < 8);
}

void moveForward()
{
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(770, mouse.velocity, mouse.maxVelocity, mouse.maxVelocity, mouse.acceleration, mouse.deceleration);
}

void moveForwardAndStop()
{	
	//Make the robot go forward
	setDirection(0, 0);

	//Move mouse
	straight(770, mouse.velocity, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void moveBackwards()
{
	//Rotate Left
	setDirection(0, 1);
	
	//Move Mouse
	straight(640, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void moveBackwardsAndCorrect()
{
	//Rotate Left 180
	setDirection(0, 1);
	straight(640, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
	
	
	//Go Backwards
	setDirection(1, 1);	
	straight(250, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
	
	setDirection(0, 0);
	straight(130, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void rotateLeft()
{
	//Rotate Left
	setDirection(0, 1);
	
	//Move Mouse
	straight(307, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}

void rotateRight()
{
	//Rotate Right
	setDirection(1, 0);
	
	//Move Mouse
	straight(320, 0, mouse.maxVelocity, 0, mouse.acceleration, mouse.deceleration);
}






float getLeftIR()
{
	return linearizeIRSensorValues( getIRSensorValue(&PORTD, PD7, 6) );
}

float getRightIR()
{
	return linearizeIRSensorValues( getIRSensorValue(&PORTC, PC6, 4) );
}

float getFrontIR()
{
	return linearizeIRSensorValues( getIRSensorValue(&PORTD, PD4, 5) );
}

int getIRSensorValue(volatile uint8_t *port, uint8_t pin, int analogChannel)
{
	//Turn on Emitter
	(*port) |= (1 << pin);
	
	_delay_us(80);
	
	int emit = ReadADC(analogChannel);
	
	//Turn off Emitter
	(*port) &= ~(1 << pin);
	
	_delay_us(80);
	
	return emit;	
}

/* Setup Analog To Digital Converter */
void setupADC()
{
	//Setup Registers
	ADMUX = (1 << REFS0);// | (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
	ADCSRA = (1 << ADEN) | (ADPS0) | (1 << ADPS1) | (1 << ADPS2);	
	
	//IR Triggers
	DDRD |= (1 << PORTD4) | (1 << PORTD7);
	DDRC |= (1 << PORTC6);
}

uint16_t ReadADC(uint8_t ch)
{
   //Select ADC Channel ch must be 0-7
   ch=ch&0b00000111;
   ADMUX= (1 << REFS0) | ch;

   //Start Single conversion
   ADCSRA|=(1<<ADSC);
	
   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   ADCSRA|=(1<<ADIF);

   return(ADC);
}

float linearizeIRSensorValues(float input)
{
	return -6.0220498115*log( input ) + 43.5411429577;
}

