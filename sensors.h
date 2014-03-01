#ifndef SENSORS_H
#define SENSORS_H

//Function Prototypes
void setupADC(void);
uint16_t ReadADC(uint8_t ch);
int getIRSensorValue(volatile uint8_t *port, uint8_t pin, int analogChannel);

#endif
