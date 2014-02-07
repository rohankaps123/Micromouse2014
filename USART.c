#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
 
//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
 
 
void USART_init(void){
 
 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);
}
 
unsigned char USART_receive(void){
 
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
 
}
 
void USART_send( unsigned char data){
 
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
 
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