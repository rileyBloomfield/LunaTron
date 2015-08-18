#include "Arduino.h"
#include "OpticalFlowSensor.h"
#include "ps2.h"

OpticalFlowSensor::OpticalFlowSensor(int clkPin, int dataPin, int res, int scale): clockPin(clkPin), dataPin(dataPin), resolution(res), scale(scale) {
  device = new PS2(clockPin, dataPin);
  
  //Initialize PS/2 device
  device->write(0xff);  // reset
  device->read();  // ack byte
  device->read();  // blank
  device->read();  // blank
  device->write(0xf0);  // set mode to remote
  device->read();  // ack
  delayMicroseconds(100);
  
  setResolution(resolution);
  setScale(scale);
};

OpticalFlowSensor::~OpticalFlowSensor() {
  delete device;
  device = NULL;
};

char OpticalFlowSensor::setResolution(int res) {
  device->write(0xE8); //set resolution
  device->read();  // ack byte
  device->write(res); // 00 1, 01 2, 02 3, 03 4
  char ack = device->read();  // ack byte
  delayMicroseconds(100);
  return ack;
};

char OpticalFlowSensor::setScale(int scale) {
  device->write(scale); //set scale, E7 2:1 && E6 1:1
  char ack = device->read();  // ack byte
  delayMicroseconds(100);
  return ack;
};

char OpticalFlowSensor::getDistances(char* array, int size) {
  //device->write(0xeb);  // give me data!
  //device->read();
  //device->read();
  //device->read();
  //device->read();
  //Mouse is now zeroed
  //delay(100);
  //send back how much movement in 50 ms
  device->write(0xeb);  // give me data!
  char ack = device->read();
  array[0] = device->read();
  array[1] = device->read(); 
  array[2] = device->read();
  return ack;
};
