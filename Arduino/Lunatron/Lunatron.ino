#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"
#include "state.h"
#include "OpticalFlowSensor.h"

//rosserial libraries
#include <ros.h>
#include <std_msgs/String.h>

//functions
void publishCurrent();
void publishLoad();
void publishEncoder();
void publishOdometry();
void driveAction(const std_msgs::String& action);
void dutyAction(const std_msgs::String& action);

//ros node
ros::NodeHandle nh;

//messages
std_msgs::String currentData, loadData, encoderData, opticalFlowData;

//subscribers
ros::Subscriber<std_msgs::String> driveCommand("driveCommands", &driveAction);
ros::Subscriber<std_msgs::String> dutyCommand("dutyCommands", &dutyAction);

//publishers
ros::Publisher currentSensors("currentSensors", &currentData);
ros::Publisher loadSensors("loadSensors", &loadData);
ros::Publisher encoderSensors("encoderSensors", &encoderData);
ros::Publisher opticalFlowSensors("opticalFlowSensors", &opticalFlowData);

//charArray buffers
char currentCharArray[60] = "1,2,3,4,5,6";
char loadCharArray[60] = "1,2,3,4,5,6";
char encoderCharArray[60] = "1,2,3,4,5,6";
char odometry[3];

OpticalFlowSensor* opticalFlowSensor;

void setup()
{
  //init ros node
  nh.initNode();
  
  //publishers
  nh.advertise(currentSensors);
  nh.advertise(loadSensors);
  nh.advertise(encoderSensors);
  nh.advertise(opticalFlowSensors);
  
  //subscirbers
  nh.subscribe(driveCommand);
  nh.subscribe(dutyCommand);
  
  //init serial comm
  Serial.begin(57600);
  
  opticalFlowSensor = new OpticalFlowSensor(14, 15, 0x03, 0xE7);
}

void loop()
{
  //Obtain current sensor data and publish
  publishCurrent();
  
  //Obtain load cells and publish
  publishLoad();
  
  //Obtain encoders and publish
  publishEncoder();
  
  //Obtain optical flow sensor reading
  publishOdometry();
  
  //mandatory spin
  nh.spinOnce();
  
  delay(200);
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

void publishOdometry() {
    opticalFlowSensor->getDistances(odometry, sizeof(odometry));
    
   /* char msg[64] = {0}, *m = msg; 
    String valX((int)odometry[1]);
    valX.toCharArray(m, sizeof(msg));
    m += valX.length();
    *m++ = ',';
    String valY((int)odometry[2]);
    valY.toCharArray(m, (sizeof(msg)));
    */
    
    //get data from motors
  char msg[64] = {0}, *m = msg; 
  for (int i = 1; i < 3; i++) {
    dtostrf((float)odometry[i]/636.86/0.2, 4, 2, m);
    while(*m) { m++; }
    *m++ = ',';
  }
  *(--m) = 0;
    
    opticalFlowData.data = msg;
    opticalFlowSensors.publish(&opticalFlowData);
    nh.spinOnce();
}
