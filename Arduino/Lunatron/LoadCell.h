#ifndef LoadCell_h
#define LoadCell_h

#include "Arduino.h"

class LoadCell
{
public:
	static LoadCell FL, ML, BL, FR, MR, BR;

	~LoadCell();
        float getLoad();
        
private:
	LoadCell(int pin, float loadMin, float loadMax, float loadSlope, float loadIntercept);
        float _loadMin, _loadMax, _loadSlope, _loadIntercept;
        int _pin;
};


#endif
