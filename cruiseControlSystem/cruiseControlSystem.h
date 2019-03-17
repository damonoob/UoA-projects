#ifndef CRUISECONTROLSTSTEM_H
#define CRUISECONTROLSTSTEM_H

#include <stdbool.h>

float saturateThrottle(float throttleIn, int* saturate);
float regulateThrottle(int isGoingOn, float cruiseSpeed, float vehicleSpeed);
int speedInRange(float speed);
int pedalOn(float pedalPercentage);
float cruiseSpeedAdjust(float speed, int increase);
float setSpeed(float speed);

typedef enum {
	OFF = 1, ON, STDBY, DISABLE
} StateType;

void _StateType(StateType *lhs, StateType rhs);
int _eq_StateType(StateType lhs, StateType rhs);
char* _StateType_to_text(StateType state);
void _text_to_StateType(StateType *state, char *text);
int _check_StateType(char *text);

#endif
