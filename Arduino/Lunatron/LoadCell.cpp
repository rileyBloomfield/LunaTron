#include "Arduino.h"
#include "LoadCell.h"

LoadCell LoadCell::BL(A8, 1, 0);
LoadCell LoadCell::ML(A9, 1, 0);
LoadCell LoadCell::FL(A10, 1, 0);
LoadCell LoadCell::FR(A11, 1, 0);
LoadCell LoadCell::MR(A12, 1, 0);
LoadCell LoadCell::BR(A13, 1, 0);

LoadCell::LoadCell(int pin, float loadScale, float loadOffset) : _pin(pin), _loadScale(loadScale), _loadOffset(loadOffset) {
	pinMode(pin, INPUT);
}

LoadCell::~LoadCell() {}
