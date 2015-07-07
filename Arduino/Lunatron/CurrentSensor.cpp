#include "Arduino.h"
#include "CurrentSensor.h"

CurrentSensor CurrentSensor::BL(A2, 1, 1, 1, 1);
CurrentSensor CurrentSensor::ML(A3, 1, 1, 1, 1);
CurrentSensor CurrentSensor::FL(A4, 1, 1, 1, 1);
CurrentSensor CurrentSensor::FR(A5, 1, 1, 1, 1);
CurrentSensor CurrentSensor::MR(A6, 1, 1, 1, 1);
CurrentSensor CurrentSensor::BR(A7, 1, 1, 1, 1);

CurrentSensor::CurrentSensor(int pin, float currentScalePos, float currentScaleNeg, float currentOffsetPos, float currentOffsetNeg):
_pin(pin), _currentScalePos(currentScalePos), _currentScaleNeg(currentScaleNeg), _currentOffsetPos(currentOffsetPos), _currentOffsetNeg(currentOffsetNeg)
{
	_posxIntercept = -_currentOffsetPos / _currentScalePos;
	_negxIntercept = -_currentOffsetNeg / _currentScaleNeg;
	pinMode(pin, INPUT);
}

CurrentSensor::~CurrentSensor() {}

//Method applies scale to current reading to transform raw reading to units of amps
int CurrentSensor::operator()(int reading, State &direction) {
  if (direction == State::BRAKE)
     return reading * 0;
  if (direction == State::FWD)
     return reading * currentScalePos;
  if (direction == State::REV)
     return reading * currentScaleNeg;
}











