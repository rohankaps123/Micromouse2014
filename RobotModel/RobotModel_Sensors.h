#ifndef ROBOTMODEL_SENSORS_H
#define ROBOTMODEL_SENSORS_H

//Sensor Functions
float getLeftIR(void);
float getRightIR(void);
float getFrontIR(void);
int getIRSensorValue(volatile uint8_t *port, uint8_t pin, int analogChannel);
int getPotSensorValue(int analogChannel);
int isButtonPushed(int analogChannel);

void setupADC(void);
uint16_t ReadADC(uint8_t ch);
float linearizeIRSensorValues(float input);

#endif