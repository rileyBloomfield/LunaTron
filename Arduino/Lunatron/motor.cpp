#include "Arduino.h"
#include "motor.h"
#include "state.h"
#include "LoadCell.h"

#define PWM_FREQ 490

Motor Motor::BL(2, 22, 24, State::BRAKE, LoadCell::BL, A2, 0.0351, 0.0224);
Motor Motor::ML(3, 28, 30, State::BRAKE, LoadCell::ML, A3, 1, 0);
Motor Motor::FL(4, 34, 36, State::BRAKE, LoadCell::FL, A4, 0.0214, 0.0427);
Motor Motor::FR(5, 40, 42, State::BRAKE, LoadCell::FR, A5, 0.027, 0.0414);
Motor Motor::MR(6, 46, 48, State::BRAKE, LoadCell::MR, A6, 0.0244, -0.1317);
Motor Motor::BR(7, 53, 51, State::BRAKE, LoadCell::BR, A7, 0.0214, -0.2107);

Motor* Motor::location[6] = { &Motor::BL, &Motor::ML, &Motor::FL, &Motor::FR, &Motor::MR, &Motor::BR };

Motor::Motor(int pin, int A, int B, State& dir, LoadCell& location, int CurrentPin, float CurrentScale, float CurrentOffset)
{
	//Set Motor Values
	_PWMpin = pin;
	_dutyCycle = 1;
	_pinA = A;
	_pinB = B;
	_direction = &dir;
	_LoadSensor = &location;
	pinMode(_PWMpin, OUTPUT);
	analogWrite(_PWMpin, _dutyCycle * 255);
	pinMode(_pinA, OUTPUT);
	pinMode(_pinB, OUTPUT);
	setDirection(*_direction);

	//Set Current Sensor
	_currentPin = CurrentPin;
	_currentScale = CurrentScale;
	_currentOffset = CurrentOffset;
	pinMode(_currentPin, INPUT);
}

Motor::~Motor()
{	
}

void Motor::setDuty(float duty)
{
	_dutyCycle = duty;
	analogWrite(_PWMpin, _dutyCycle * 255);
}

void Motor::setDirection(State &state)
{
	_direction = &state;
	digitalWrite(_pinA, state.readA());
	digitalWrite(_pinB, state.readB());
}

void Motor::switchDirection()
{
	_direction = &(_direction->opposite());
}

State& Motor::getDirection()
{
	return *_direction;
}

float Motor::getCurrent()
{
	int sum = 0;
	sumvar = 0;
	for (int i = 0; i < POLL_COUNT; i++) 
	{ 
		sum += analogRead(_currentPin);
		sumvar += analogRead(_currentPin)*analogRead(_currentPin);
	}
	sumvar = sqrt(abs((sumvar - sum*sum / POLL_COUNT) / POLL_COUNT));
	return (float)(sum / POLL_COUNT);
}
