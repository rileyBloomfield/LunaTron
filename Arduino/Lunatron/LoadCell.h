#ifndef LoadCell_h
#define LoadCell_h

#include "Arduino.h"

class LoadCell
{
public:
	static LoadCell FL, ML, BL, FR, MR, BR;
	~LoadCell();
private:
	LoadCell(int pin, float loadScale, float loadOffset);
	int _pin;
	float _loadScale, _loadOffset;
};


#endif
