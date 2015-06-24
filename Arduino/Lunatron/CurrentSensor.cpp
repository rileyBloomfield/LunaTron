#include "Arduino.h"
#include "CurrentSensor.h"

CurrentSensor CurrentSensor::BL(A2, 0.0382, 0.0288, 0.127, 0.1218);
CurrentSensor CurrentSensor::ML(A3, 1, 1, 0, 0);
CurrentSensor CurrentSensor::FL(A4, 0.0311, 0.0202, 0.023, -0.0028);
CurrentSensor CurrentSensor::FR(A5, 0.0355, 0.0268, 0.0125, 0.0824);
CurrentSensor CurrentSensor::MR(A6, 0.0252, 0.0242, -0.1113, 0.496);
CurrentSensor CurrentSensor::BR(A7, 0.0218, 0.0214, -0.1849, 0.1059);

CurrentSensor::CurrentSensor(int Pin, float CurrentScalepos, float CurrentScaleneg, float CurrentOffsetpos, float CurrentOffsetneg)
{
	Pin = Pin;
	_CurrentScalepos = CurrentScalepos;
	_CurrentScaleneg = CurrentScaleneg;
	_CurrentOffsetpos = CurrentOffsetpos;
	_CurrentOffsetneg = CurrentOffsetneg;
	_posxintercept = -_CurrentOffsetpos / _CurrentScalepos;
	_negxintercept = -_CurrentOffsetneg / _CurrentScaleneg;
	pinMode(Pin, INPUT);
}

CurrentSensor::~CurrentSensor()
{

}

float CurrentSensor::operator()(float reading)
{
  
  return reading;
  /*
	if (reading>_negxintercept && reading< _posxintercept)
	{
		return 0;
	}
	else if (reading < _negxintercept)
	{
		return _CurrentScaleneg*reading + _CurrentOffsetneg;
	}
	else
	{
		return _CurrentScalepos*reading + _CurrentOffsetpos;
	}
*/
}
