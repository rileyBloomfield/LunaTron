//#include <Wire.h>

#ifndef Comm_h
#define Comm_h

#include <Arduino.h>
#include <string.h>

class TWI_Comm
{
	public:
	static TWI_Comm TWI_PORT;
	TWI_Comm();
	~TWI_Comm();
	void SendTransmission(int device, char data);
	void ReceiveTransmission(int device, int size);
	char data[8];

	private:

};



#endif // !Comm_h
