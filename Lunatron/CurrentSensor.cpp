#include "Arduino.h"
#include "CurrentSensor.h"

CurrentSensor CurrentSensor::BL(A2, 1, 0);
CurrentSensor CurrentSensor::ML(A3, 1, 0);
CurrentSensor CurrentSensor::FL(A4, 1, 0);
CurrentSensor CurrentSensor::FR(A5, 1, 0);
CurrentSensor CurrentSensor::MR(A6, 1, 0);
CurrentSensor CurrentSensor::BR(A7, 1, 0);

CurrentSensor::CurrentSensor(int Pin, float CurrentScale, float CurrentOffset)
{
	_Pin = Pin;
	_CurrentScale = CurrentScale;
	_CurrentOffset = CurrentOffset;
	pinMode(_Pin, INPUT);
}

CurrentSensor::~CurrentSensor()
{

}
