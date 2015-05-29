#include "Arduino.h"
#include "LoadCell.h"

LoadCell LoadCell::BL(A8, 1, 0);
LoadCell LoadCell::ML(A9, 1, 0);
LoadCell LoadCell::FL(A10, 1, 0);
LoadCell LoadCell::FR(A11, 1, 0);
LoadCell LoadCell::MR(A12, 1, 0);
LoadCell LoadCell::BR(A13, 1, 0);

LoadCell::LoadCell(int Pin, float LoadScale, float LoadOffset)
{
	_Pin = Pin;
	_Loadscale = LoadScale;
	_LoadOffset = LoadOffset;
	pinMode(_Pin, INPUT);
}

LoadCell::~LoadCell()
{

}
