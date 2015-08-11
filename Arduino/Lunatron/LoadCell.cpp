#include "Arduino.h"
#include "LoadCell.h"

LoadCell LoadCell::BL(A8, 1, 0);
LoadCell LoadCell::ML(A9, 1, 0);
LoadCell LoadCell::FL(A10, 1, 0);
LoadCell LoadCell::BR(A11, 1, 0);
LoadCell LoadCell::MR(A12, 1, 0);
LoadCell LoadCell::FR(A13, 1, 0);

LoadCell::LoadCell(int pin, float loadScale, float loadOffset) : _pin(pin), _loadScale(loadScale), _loadOffset(loadOffset) {
	pinMode(pin, INPUT);
}

LoadCell::~LoadCell() {}

float LoadCell::operator()(int reading) {
  return ((0.0006*reading*reading*reading) - (0.4521*reading*reading) + (118.76*reading) - 9300)/100;
}












