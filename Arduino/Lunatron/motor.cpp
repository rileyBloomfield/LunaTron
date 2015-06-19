#include "Arduino.h"
#include "motor.h"
#include "state.h"
#include "CurrentSensor.h"
//#include "Comm.h"

Motor Motor::BL(2, 22, 24, State::BRAKE, CurrentSensor::BL);
Motor Motor::ML(3, 28, 30, State::BRAKE, CurrentSensor::ML);
Motor Motor::FL(4, 34, 36, State::BRAKE, CurrentSensor::FL);
Motor Motor::FR(5, 40, 42, State::BRAKE, CurrentSensor::FR);
Motor Motor::MR(6, 46, 48, State::BRAKE, CurrentSensor::MR);
Motor Motor::BR(7, 53, 51, State::BRAKE, CurrentSensor::BR);

Motor* Motor::location[6] = { &Motor::BL, &Motor::ML, &Motor::FL, &Motor::FR, &Motor::MR, &Motor::BR };

Motor::Motor(int pin, int A, int B, State& dir,  CurrentSensor& location)
{
	//Set Motor Values
	_PWMpin = pin;
	_dutyCycle = 1;
	_pinA = A;
	_pinB = B;
	_direction = &dir;
	//_comm = &TWI_Comm::TWI_PORT;
	_currsensor = &location;
	pinMode(_PWMpin, OUTPUT);
	analogWrite(_PWMpin, _dutyCycle * 255);
	pinMode(_pinA, OUTPUT);
	pinMode(_pinB, OUTPUT);
	setDirection(*_direction);
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
	float sum = 0;
	for (int i = 0; i < POLL_COUNT; i++)
	{
		sum += (*_currsensor)(analogRead(_currsensor->Pin));
	}
	return sum/POLL_COUNT;
}

int Motor::getEncCount()
{
	int count = 0;
	//_comm->ReceiveTransmission(EncDecAddr, DataSize);
	//count = _comm->data[0];
	count = count << 8;
	//count |= _comm->data[1];
	//_comm->SendTransmission(EncDecAddr, CLEAR);
	return count;
}

void Motor::ZeroEncCount()
{
	//_comm->SendTransmission(EncDecAddr, CLEAR);
}
