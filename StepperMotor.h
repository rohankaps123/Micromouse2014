#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

//If OCR0A is set to 1, how many interrupts would there be in one second?
#define TIMER_FREQUENCY 250000

typedef struct StepperMotor
{
	//What should the motor execute
	volatile int mode;
	
	//How many steps so far?
	long accelCount;
	long stepCount;
	long decelCount;
	
	//Total Steps for each action
	int totalAccelSteps;
	int totalRunSteps;
	int totalDecelSteps;
	
	//How long to delay the motor between steps (TimerFrequency/speed)
	float currentStepDelay;
	
	//Minimum delay that will be calculated for maximumSpeed
	int minDelay;
	
	//Number of steps required for deceleration
	float decelSteps;
	int decelStop;
	
	int delayCounter;
	
	
} StepperMotor;

#define ACCEL 	0
#define DECEL 	1
#define RUN		2
#define STOP	3

//Function Prototypes
void setupStepperMotor(void);
StepperMotor controlMotor(long stepTarget, int inSpeed, int maxSpeed, int outSpeed, int accel, int decel);
void pulseMotor(volatile uint8_t *port, uint8_t pin);
void enableDrive(int left, int right);
void accelerateMotor(struct StepperMotor *motor, int continueTo);
void runMotor(struct StepperMotor *motor, int continueTo);
void decelerateMotor(struct StepperMotor *motor, int continueTo);
int calculateNextDelay(struct StepperMotor *motor,volatile uint8_t *shutoffPort);
int eightBitTimerFix(struct StepperMotor *motor);
void turnOnTimers(int one, int two);
void setDirection(int left, int right);
#endif