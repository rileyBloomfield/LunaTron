#include "Arduino.h"
#include "LoadCell.h"

LoadCell LoadCell::FR(A13, 144, 340, 0.0984, -5.5682);
LoadCell LoadCell::MR(A12, 144, 430, 0.0952, -0.968);
LoadCell LoadCell::BR(A11, 144, 365, 0.0986, -1.6729);
LoadCell LoadCell::BL(A8, 142, 350, 0.1008, -7.6508);
LoadCell LoadCell::ML(A9, 146, 440, 0.0945, -7.2166);
LoadCell LoadCell::FL(A10, 155, 320, 0.0876, -11.287);




LoadCell::LoadCell(int pin, float loadMin, float loadMax, float loadSlope, float loadIntercept) 
  : _pin(pin), _loadMin(loadMin), _loadMax(loadMax),_loadSlope(loadSlope), _loadIntercept(loadIntercept) {
	pinMode(pin, INPUT);
}

LoadCell::~LoadCell() {}

float LoadCell::getLoad() {
  int x = analogRead(_pin);
  return (x < _loadMin || x > _loadMax) ? -1 :  x * _loadSlope + _loadIntercept;
}












