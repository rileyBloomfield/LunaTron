/*#include "Comm.h"

TWI_Comm TWI_Comm::TWI_PORT;

TWI_Comm::TWI_Comm(){}

TWI_Comm::~TWI_Comm(){}

void TWI_Comm::SendTransmission(int device, char data)
{
	Wire.beginTransmission(device);
	Wire.write(byte(data));
	Wire.endTransmission();
	Serial.print("Sent:");
	Serial.print(data);
	Serial.println();
}

void TWI_Comm::ReceiveTransmission(int device, int size)
{
	Wire.requestFrom(device, size);
	for (int i = 0; i < size; i++)
	{
		data[i] = Wire.read();
	}
	return;
}*/
