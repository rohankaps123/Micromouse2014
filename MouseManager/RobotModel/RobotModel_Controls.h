#ifndef ROBOTMODEL_CONTROLS_H
#define ROBOTMODEL_CONTROLS_H

void straight(long stepTarget, int inSpeed, int maxSpeed, int exitSpeed, int accel, int decel);
void smoothTurn(float degrees, float midRadius, int inSpeed, float maxSpeed, int exitSpeed, int accel, int decel);
float getOffsetError(void);
void startTimer(void);
float getRightError(void);
float getLeftError(void);
#endif