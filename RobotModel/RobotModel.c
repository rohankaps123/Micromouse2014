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

volatile Mouse mouse;

int isWallForward()
{
	int value = getIRSensorValue(&PORTD, PD4, 5);
	return 0;
}

int isWallRight()
{
	int value = getIRSensorValue(&PORTC, PC6, 4);
	return 0;
}

int isWallLeft()
{
	//Fetch Sensor Value on left side
	int value = getIRSensorValue(&PORTD, PD7, 6);
	
	return 0;
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

