#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "USART.h"

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
 
//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
 
 
void USART_init(void){
 
 UBRR1H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR1L = (uint8_t)(BAUD_PRESCALLER);
 UCSR1B = (1<<RXEN1)|(1<<TXEN1);
 UCSR1C = (3<<UCSZ10);
}
 
unsigned char USART_receive(void){
 
 while(!(UCSR1A & (1<<RXC1)));
 return UDR1;
 
}
 
void USART_send( unsigned char data){
 
 while(!(UCSR1A & (1<<UDRE1)));
 UDR1 = data;
 
}
 
void USART_putstring(char* StringPtr){
 
while(*StringPtr != 0x00){
 USART_send(*StringPtr);
 StringPtr++;}
 
}

char str[16];
void printlnNum(double value)
{
	sprintf(str,"%f",value);
	USART_putstring(str);    //Pass the string to the USART_putstring function and sends it over the serial  
	USART_putstring("\n\r");
}

void printNum(float value)
{
	sprintf(str,"%f",value);
	USART_putstring(str);    //Pass the string to the USART_putstring function and sends it over the serial
}

void print(char* string)
{
	USART_putstring(string);
}

void printInt(int value)
{
	char str[16];
	itoa(value, str, 10);
	
	//Add Leading Zero's for consistency
	for(int i = 1; i < 4-strlen(str); i++)
	{
		USART_putstring("0");
	}
	USART_putstring(str);
}