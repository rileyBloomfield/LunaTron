#include "ps2.h"
class OpticalFlowSensor {
  private:
    int resolution, scale;
    int clockPin, dataPin;
    PS2* device;
  public:
    OpticalFlowSensor(int clkPin, int dataPin, int res, int scale);
    ~OpticalFlowSensor();
    char setResolution(int res);
    char setScale(int scale);
    char getDistances(char* array, int size);
};
