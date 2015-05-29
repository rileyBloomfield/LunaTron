/*
	state.h - Library for controlling motor direction pins
	Created by Nicole Devos, June 7, 2014
*/
#ifndef State_h
#define State_h

#include "Arduino.h"

class State
{
public:
	static State FWD, REV, BRAKE;
	bool readA(void);
	bool readB(void); 
	~State();
	bool operator==(State& src);
	State& opposite() const;
private:
	State(const bool A, const bool B);
	State* _opposite;
	bool _A;
	bool _B;

	static bool oppositesAssigned;
	static bool assignOpposites();
};

#endif
