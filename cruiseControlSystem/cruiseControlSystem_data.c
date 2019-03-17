#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cruiseControlSystem.h"

StateType offState = OFF, onState = ON, stdbyState = STDBY, disableState = DISABLE;

/*
DESCRIPTION: Saturate the throttle command to limit the acceleration.
PARAMETERS: throttleIn - throttle input
            saturate - true if saturated, false otherwise
RETURNS: throttle output (ThrottleCmd)
*/
float saturateThrottle(float throttleIn, int* saturate)
{
	static const float THROTTLESATMAX = 45.0;
	if (throttleIn > THROTTLESATMAX) {
		*saturate = 1;
		return THROTTLESATMAX;
	}
	else if (throttleIn < 0) {
		*saturate = 1;
		return 0;
	}
	else {
		*saturate = 0;
		return throttleIn;
	}
}

/*
DESCRIPTION: Saturate the throttle command to limit the acceleration.
PARAMETERS: isGoingOn - true if the cruise control has just gone into the ON state 
                        from another state; false otherwise
            saturate - true if saturated, false otherwise
RETURNS: throttle output (ThrottleCmd)
*/
float regulateThrottle(int isGoingOn, float cruiseSpeed, float vehicleSpeed)
{
	static const float KP = 8.113;
	static const float KI = 0.5;
	static int saturate = 1;
	static float iterm = 0;
	
	if (isGoingOn == 1) {
		iterm = 0;	// reset the integral action
		saturate = true;	
	}
	float error = cruiseSpeed - vehicleSpeed;
	float proportionalAction = error * KP;
	if (saturate)
		error = 0;	// integral action is hold when command is saturated
	iterm = iterm + error;
	float integralAction = KI * iterm;
	return saturateThrottle(proportionalAction + integralAction, &saturate);
}

/*
DESCRIPTION: Checks if the speed of the car is within the range
PARAMETERS: speed  - current speed of the car 
RETURNS: 1 if speed is in the range, 0 otherwise
*/
int speedInRange(float speed)
{
	static const float speedMin = 30.0;
	static const float speedMax = 150.0;
	if(speed >= speedMin && speed <= speedMax){
		return 1;
	}else{
		return 0;
	}
}

/*
DESCRIPTION: Checks if a pedal is pressed. i.e. more than 3 percent
PARAMETERS: pedalPercentage  - current position of the pedal
RETURNS: 1 if pedal is pressed, 0 otherwise
*/
int pedalOn(float pedalPercentage)
{
	static const float pedalThresPercent = 3.0;
	return pedalPercentage > pedalThresPercent;
}


/*
DESCRIPTION: Adjust the cruising speed 
PARAMETERS:  speed  - current cruising speed
			 increase - whether increase or decrease the speed
			 		   1 for increase , 0 for decrease
RETURNS: 	new cruising speed
*/
float cruiseSpeedAdjust(float speed, int increase)
{
	static const float speedMin = 30.0;
	static const float speedMax = 150.0;
	static const float speedInc = 2.5;
	float speedOut = speed;
	if (increase == 1) {
		if (speed + speedInc < speedMax){
			speedOut += speedInc;
		} else {
			speedOut = speedMax;
		}
	} else {
		if (speed - speedInc > speedMin){
			speedOut -= speedInc;
		} else {
			speedOut = speedMin;
		}
	}
	
	return setSpeed(speedOut); 
	
}

/*
DESCRIPTION: Set the current speed to cruising speed, if speed exceeds ranges, set max/min speed as the crusing speed. 
PARAMETERS:  speed  - current speed
RETURNS: 	the new cruising speed 
*/
float setSpeed(float speed)
{
	static const float speedMin = 30.0;
	static const float speedMax = 150.0;
	float speedOut = speed;
	
	if (speedInRange(speed)) {
		return speedOut;
	} else if (speed < speedMin) {
		return speedMin;
	} else {
		return speedMax;
	}	
}

/*
DESCRIPTION: Assigning function of the enum type
*/
void _StateType(StateType *lhs, StateType rhs)
{
    *lhs = rhs;
}

/*
DESCRIPTION: test equal function of the enum type
*/
int _eq_StateType(StateType lhs, StateType rhs)
{
    return lhs == rhs;
}

/*
DESCRIPTION: Converting from the enum type to text
*/
char* _StateType_to_text(StateType state)
{    
    static char text;
    switch(state) {
    	case OFF:
    		text = '1';
    		break;
    	case ON:
    		text = '2';
    		break;
    	case STDBY:
    		text = '3';
    		break;
    	case DISABLE:
    		text = '4';
    		break;
    	default:
    		text = '1';
    		break;    		
    }
    return &text;
}

/*
DESCRIPTION: Converting from text to the enum type
*/
void _text_to_StateType(StateType *state, char *text)
{
    switch(*text) {
    	case '1':
    		*state = OFF;
    		break;
    	case '2':
    		*state = ON;
    		break;
    	case '3':
    		*state = STDBY;
    		break;
    	case '4':
    		*state = DISABLE;
    		break;
    	default:
    		*state = OFF;
    		break;    		
    }
}

/*
DESCRIPTION: Type sanity check of the enum type
*/
int _check_StateType(char *text)
{
    switch(*text) {
    	case '1':    		
    	case '2':
    	case '3':
    	case '4':
    		return 0;
    	default:
    		return 1;  		
    }
}


