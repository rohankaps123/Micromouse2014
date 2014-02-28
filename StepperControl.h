#ifndef STEPPERCONTROL_H
#define STEPPERCONTROL_H

//Function Prototypes
void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel);
float calculateInitialDelay(float accel);
float calculateGetDelay(float nowDelay, int stepStart, int stepEnd);
void turn(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel);
void setAccel(struct StepperMotor *motor, int inSpeed, int maxSpeed, int accel);
void setDecel(struct StepperMotor *motor, int maxSpeed, int exitSpeed, int decel);
int motorStillRunning(struct StepperMotor *motor);
#endif