#ifndef LoadCell_h
#define LoadCell_h

#include "Arduino.h"

class LoadCell
{
public:
	static LoadCell FL, ML, BL, FR, MR, BR;
	~LoadCell();
private:
	LoadCell(int Pin, float LoadScale, float LoadOffset);
	int _Pin;
	float _Loadscale;
	float _LoadOffset;
};


#endif
