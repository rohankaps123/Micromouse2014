#include <avr/io.h>
#include <util/delay.h>

#include "RobotModel_Sensors.h"

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

int getPotSensorValue(int analogChannel)
{ 
	return ReadADC(analogChannel);
}

int isButtonPushed(int analogChannel)
{
	return !(ReadADC(analogChannel) > 512);
}

void turnOnLeds(int num)
{
	if((num & 4) == 4)	
		PORTE |= (1 << PE6);
	
	else
		PORTE &= ~(1 << PE6);
		
	if((num & 2) == 2)
		PORTB |= (1 << PB5);
		
	else
		PORTB &= ~(1 << PB5);
		
	if((num & 1) == 1)
		PORTB |= (1 << PB4);
		
	else
		PORTB &= ~(1 << PB4);
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
	
	//LED's
	DDRB |= (1 << PORTB4);
	DDRB |= (1 << PORTB5);
	DDRE |= (1 << PORTE6);	
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

