#ifndef CurrentSensor_h
#define CurrentSensor_h

#include "Arduino.h"

class CurrentSensor
{
//Static
public:
	static CurrentSensor FL, ML, BL, FR, MR, BR;

//Constructors
public:
	~CurrentSensor();

private:
	CurrentSensor(int Pin, float CurrentScalepos, float CurrentScaleneg, float CurrentOffsetpos, float CurrentOffsetneg);

//Data
public:
	int Pin;

private:
	float _CurrentScalepos;
	float _CurrentScaleneg;
	float _CurrentOffsetpos;
	float _CurrentOffsetneg;
	
	float _posxintercept;
	float _negxintercept;

//Methods
public:
	float operator()(float reading);
	
};


#endif