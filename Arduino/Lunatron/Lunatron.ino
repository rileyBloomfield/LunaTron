#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"
#include "state.h"

//rosserial libraries
#include <ros.h>
#include <std_msgs/String.h>

//functions
void publishCurrent();
void publishLoad();
void publishEncoder();
void driveAction(const std_msgs::String& action);
void dutyAction(const std_msgs::String& action);

//ros node
ros::NodeHandle nh;

//messages
std_msgs::String currentData, loadData, encoderData;

//subscribers
ros::Subscriber<std_msgs::String> driveCommand("driveCommands", &driveAction);
ros::Subscriber<std_msgs::String> dutyCommand("dutyCommands", &dutyAction);

//publishers
ros::Publisher currentSensors("currentSensors", &currentData);
ros::Publisher loadSensors("loadSensors", &loadData);
ros::Publisher encoderSensors("encoderSensors", &encoderData);

//charArray buffers
char currentCharArray[60] = "1,2,3,4,5,6";
char loadCharArray[60] = "1,2,3,4,5,6";
char encoderCharArray[60] = "1,2,3,4,5,6";

void setup()
{
  //init ros node
  nh.initNode();
  
  //publishers
  nh.advertise(currentSensors);
  nh.advertise(loadSensors);
  nh.advertise(encoderSensors);
  
  //subscirbers
  nh.subscribe(driveCommand);
  nh.subscribe(dutyCommand);
  
  //init serial comm
  Serial.begin(57600);
  
  Motor::location[1]->setDirection(State::FWD);
}

void loop()
{
  //Obtain current sensor data and publish
  publishCurrent();
  
  //Obtain load cells and publish
  publishLoad();
  
  //Obtain encoders and publish
  publishEncoder();
  
  //mandatory spin
  nh.spinOnce();

  delay(2000);
}

void publishCurrent() {
  //get data from motors
  char msg[64] = {0}, *m = msg; 
  for (int i = 0; i < 6; i++) {
    dtostrf(Motor::location[i]->getCurrent(), 4, 2, m);
    while(*m) { m++; }
    *m++ = ',';
  }
  *(--m) = 0;
  //publish
  currentData.data = msg;
  currentSensors.publish(&currentData);
  nh.spinOnce();
}

void publishLoad() {
  //get data from motors
  char msg[64] = {0}, *m = msg; 
  for (int i = 0; i < 6; i++) {
    dtostrf(Motor::location[i]->getLoad(), 4, 2, m);
    while(*m) { m++; }
    *m++ = ',';
  }
  *(--m) = 0;
  //publish
  loadData.data = msg;
  loadSensors.publish(&loadData);
  nh.spinOnce();
}

void publishEncoder() {
    nh.spinOnce();
}


void driveAction(const std_msgs::String& action) {
  char switchChar = action.data[0];
   switch(switchChar){
    case '0': //STOP
      for (int h = 0; h < 6; h++)
        Motor::location[h]->setDirection(State::BRAKE);
      break;
    case '1': //FORWARD
      for (int h = 0; h < 3; h++)
        Motor::location[h]->setDirection(State::FWD);
      for (int h = 3; h < 6; h++)
        Motor::location[h]->setDirection(State::REV);
      break;
    case '2': //REVERSE
      for (int h = 0; h < 3; h++)
        Motor::location[h]->setDirection(State::REV);
      for (int h = 3; h < 6; h++)
        Motor::location[h]->setDirection(State::FWD);
      break;
    case '3': //LEFT
      for (int h = 0; h < 6; h++)
        Motor::location[h]->setDirection(State::REV);
      break;
    case '4': //RIGHT
      for (int h = 0; h < 6; h++)
        Motor::location[h]->setDirection(State::FWD);
      break;
    }
}

void dutyAction(const std_msgs::String& action) {
  char inBuff[3], *p = inBuff, dutyChar = action.data[0];
  *p++ = '0';
  *p++ = '.';
  *p++ = dutyChar;
  float duty = atof(inBuff);
  for (int i=0; i<6; i++) {
    if(duty>=0 && duty <=1)
      Motor::location[i]->setDuty(duty);
  }
}




