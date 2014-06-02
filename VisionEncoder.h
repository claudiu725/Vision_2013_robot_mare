#ifndef VisionEncoder_h
#define VisionEncoder_h

#include "Arduino.h"
#include "big_robot_constants.h"
#include "VisionSensor.h"

class VisionEncoder {
  public:
    void initPin(int sensorPin);
    void initRevolutionSteps(long revolutionSteps);
    void reset();
    long count();
    void doLoop();
  private:
    VisionSensor sensor;
    boolean state;
    long stepCount;
  public:
    long revolutionSteps;
};

#endif
