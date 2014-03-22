#ifndef ROBOTMODEL_SENSORS_H
#define ROBOTMODEL_SENSORS_H

typedef struct Sensor
{
	volatile float value;
	volatile float previousValue;
	
	volatile uint8_t *port;
} Sensor;

//Sensor Functions
float getLeftIR(void);
float getRightIR(void);
float getFrontLeftIR(void);
float getFrontRightIR(void);
int getIRSensorValue(volatile uint8_t *port, uint8_t pin, int analogChannel);
int getPotSensorValue(int analogChannel);
int isButtonPushed(int analogChannel);

void setupADC(void);
uint16_t ReadADC(uint8_t ch);
float linearizeIRSensorValues(float input);
float linearizeIRSensorValues_Front(float input);
float getFrontAngle(void);
float getFrontLeftIRLong(void);
float getFrontRightIRLong(void);

void calibrateGyro(void);
int updateGyroValue(void);
#endif