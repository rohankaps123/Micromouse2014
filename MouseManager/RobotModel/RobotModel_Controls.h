#ifndef ROBOTMODEL_CONTROLS_H
#define ROBOTMODEL_CONTROLS_H

void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel);
void smoothTurn(float degrees, float midRadius, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel);
int getOffsetError(void);
void startTimer(void);
float getRightError(void);
float getLeftError(void);
#endif