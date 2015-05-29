#include "Arduino.h"
#include "state.h"


State State::FWD = State(HIGH, LOW);
State State::REV = State(LOW, HIGH);
State State::BRAKE = State(LOW, LOW);
bool State::oppositesAssigned = State::assignOpposites();

State::State(const bool A, const bool B)
{
	_A = A;
	_B = B;
}
	
State::~State()
{

}

bool State::operator==(State& src)
{
		return this==&src;
}

State& State::opposite() const
{
	return *_opposite;
}

bool State::readA(void)
{
          return _A;
}

bool State::readB(void)
{
          return _B;
}

bool State::assignOpposites()
{
	FWD._opposite = &REV;
	BRAKE._opposite = &BRAKE;
	REV._opposite = &FWD;

	return true;
}