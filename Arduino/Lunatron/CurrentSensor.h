#ifndef CurrentSensor_h
#define CurrentSensor_h

#include "Arduino.h"
#include "state.h"

class CurrentSensor
{
//Static
public:
	static CurrentSensor FL, ML, BL, FR, MR, BR;

//Constructors
public:
	~CurrentSensor();

private:
	CurrentSensor(int pin, float currentScalePos, float currentScaleNeg, float currentOffsetPos, float currentOffsetNeg);

//Data
public:
	int _pin;

private:
	float _currentScalePos, _currentScaleNeg, _currentOffsetPos, _currentOffsetNeg;
	float _posxIntercept, _negxIntercept;

//Methods
public:
	float operator()(int reading, State &direction);
};


#endif
