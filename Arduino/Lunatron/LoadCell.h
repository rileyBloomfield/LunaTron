#ifndef LoadCell_h
#define LoadCell_h

#include "Arduino.h"

class LoadCell
{
public:
	static LoadCell FL, ML, BL, FR, MR, BR;

	~LoadCell();
        float getLoad();
        int _pin;
        float operator()(int reading);
        
private:
	LoadCell(int pin, float loadScale, float loadOffset);
        float _loadScale, _loadOffset;
};


#endif
