#include "Arduino.h"
#include "CurrentSensor.h"

CurrentSensor CurrentSensor::BL(A2, 0.032, 0.0312, 0.0225, -0.1606);
CurrentSensor CurrentSensor::ML(A3, 0, 0, 0, 0);
CurrentSensor CurrentSensor::FL(A4, 0.0355, 0.0252, 0.0457, 0.0104);
CurrentSensor CurrentSensor::FR(A5, 0.0272, 0.04, -0.0405, 0.053);
CurrentSensor CurrentSensor::MR(A6, 0.035, 0.0274, -0.0194, -0.0845);
CurrentSensor CurrentSensor::BR(A7, 0.0222, 0.022, -0.0556, -0.1286);

CurrentSensor::CurrentSensor(int pin, float currentScalePos, float currentScaleNeg, float currentOffsetPos, float currentOffsetNeg):
_pin(pin), _currentScalePos(currentScalePos), _currentScaleNeg(currentScaleNeg), _currentOffsetPos(currentOffsetPos), _currentOffsetNeg(currentOffsetNeg)
{
	_posxIntercept = -_currentOffsetPos / _currentScalePos;
	_negxIntercept = -_currentOffsetNeg / _currentScaleNeg;
	pinMode(pin, INPUT);
}

CurrentSensor::~CurrentSensor() {}

//Method applies scale to current reading to transform raw reading to units of amps
float CurrentSensor::operator()(int reading, State &direction) {
  if (direction == State::BRAKE)
     return reading * 0;
  else if (direction == State::FWD)
     return reading * _currentScalePos + _currentOffsetPos;
  else if (direction == State::REV)
     return reading * _currentScaleNeg + _currentOffsetNeg;
}











