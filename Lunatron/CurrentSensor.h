#ifndef CurrentSensor_h
#define CurrentSensor_h

#include "Arduino.h"

class CurrentSensor
{
public:
	static CurrentSensor FL, ML, BL, FR, MR, BR;
	~CurrentSensor();

private:
	CurrentSensor(int Pin, float CurrentScale, float CurrentOffset);
	int _Pin;
	float _CurrentScale;
	float _CurrentOffset;
};


#endif
