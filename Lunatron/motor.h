/*
	motor.h - Library for controlling multiple motors
	Created by Nicole Devos, June 6, 2014
	NOTE: default pin for motor class is 2
		default dutyCycle for motor class is 0
*/
#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "state.h"
#include "LoadCell.h"
#include "CurrentSensor.h"

class Motor
{
	//Statics
	public:
	static Motor FL, ML, BL, FR, MR, BR;
	static Motor* location[6];
	
	private:
	static const int POLL_COUNT = 1000;

	//Data
	public:
	int sumvar;

	private:
	int _currentPin;
	float _currentScale;
	float _currentOffset;

	LoadCell* _LoadSensor;
	int _PWMpin;
	int _pinA;
	int _pinB;
	float _dutyCycle;
	State *_direction;
	

	//Constructors
	public:
	~Motor();

	private:
	Motor(int PWMpin, int pinA, int pinB, State &direction, LoadCell &location, int currentPin, float currentScale, float currentOffset);

	//Methods
	public:
	State& getDirection();
	void setDirection(State& state);
	void switchDirection();

	void setDuty(float duty);
	float getCurrent();
};

#endif
