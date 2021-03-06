#ifndef VisionArm_h
#define VisionArm_h

#include "Arduino.h"
#include "VisionStepper.h"
#include "VisionSensor.h"
#include "VisionColorSensor.h"
#include "VisionBrushless.h"
#include <elapsedMillis.h>
#include <Servo.h>

#define UP 10
#define DOWN 11
#define FORWARD 12
#define BACKWARD 13

class VisionArm {
  public:    
    Servo claw, basket, lance;
    VisionSensor fruitBarrier, fruitDistance;
    VisionColorSensor fruitColor;
    VisionSensor verticalLimiter, horizontalLimiter, horizontalAntiSlip;
    VisionStepper horizontalMotor;
    VisionBrushless verticalMotor;
    VisionSensor sensorTop, sensorMiddle, sensorBottom;
    int horizontalDirection;
    void init();
    void clawGrab();
    void clawRelease();
    void basketClose();
    void basketOpen();
    void lanceRaise();
    void lanceLower();
    void moveHorizontal(float, int);
    void moveVertical(VisionSensor& sensor);
    void doLoop();
    void stopNow();
    void disable();
    boolean isStopped();
};

#endif
