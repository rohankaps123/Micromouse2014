#ifndef USART_H
#define USART_H

//Function Prototypes
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
void printlnNum(double value);
void printNum(float value);
void print(char* string);
void printInt(int value);

#endif