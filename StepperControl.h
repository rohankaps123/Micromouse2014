#ifndef STEPPERCONTROL_H
#define STEPPERCONTROL_H

//Function Prototypes
void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel);
float calculateInitialDelay(float accel);
float calculateGetDelay(float nowDelay, int stepStart, int stepEnd);
void turn(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel);
#endif