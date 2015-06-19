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
//#include "Comm.h"

#define PWM_FREQ 490
#define EncDecAddr 0x00
#define DataSize 2
#define CLEAR 0x01

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
	
	private:
	int _PWMpin;
	int _pinA;
	int _pinB;
	float _dutyCycle;
	State* _direction;
	CurrentSensor* _currsensor;
	LoadCell* _LoadSensor;
	//TWI_Comm* _comm;
	float diameter;
	
	int _EncoderPinA;
	int _EncoderPinB;
	float _velocity;

	//Constructors
	public:
	~Motor();

	private:
	Motor(int PWMpin, int pinA, int pinB, State &direction, CurrentSensor &location);

	//Methods
	public:
	State& getDirection();
	void setDirection(State& state);
	void switchDirection();

	void setDuty(float duty);
	float getCurrent();
	int getEncCount();
	void ZeroEncCount();
	//void calcVelocity(unsigned long currentTime, unsigned long pastTime);

};

#endif
