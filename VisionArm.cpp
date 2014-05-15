#include "VisionArm.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"
    
const int delayActions = 4000;
int clawPos = 20;
int leftEncoderState;
int rightEncoderState;
int leftStepCount = 0;
int rightStepCount = 0;

void VisionArm::init()
{
  fruit.initPin(fruitSenzorPin);
  //fruitColor.initPin(fruitColorSenzorPin);
  
  horizontalMotor.init();
  horizontalMotor.initPins(horizontalArmEnablePin, horizontalArmDirectionPin, horizontalArmStepPin);
  horizontalMotor.initDelays(horizontalArmSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, horizontalArmStepSpeedCounterAcceleration, horizontalArmStepSpeedCounterSlowing);
  horizontalMotor.initStepCmRatio(horizontalArmCmStepRatio);
  
  verticalLimiter.initPin(verticalArmLimiterPin);
  verticalLimiter.setAsPullup();
  
  verticalMotor.init();
  verticalMotor.initPins(verticalArmEnablePin, verticalArmDirectionPin, verticalArmStepPin);
  verticalMotor.initDelays(verticalArmSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, verticalArmStepSpeedCounterAcceleration, verticalArmStepSpeedCounterSlowing);
  verticalMotor.initStepCmRatio(verticalArmCmStepRatio);
  
  claw.attach(clawServoPin);
}

void VisionArm::moveHorizontal(float distance, int side)
{     
  horizontalMotor.setDirectionForward();
  if(side == BACKWARD)
    horizontalMotor.toggleDirection();      
  horizontalMotor.setTargetDelay(horizontalArmSpeedDelay);
  horizontalMotor.doDistanceInCm(distance);
}

void VisionArm::moveVertical(float distance, int side)
{      
  verticalMotor.setDirectionForward();
  if(side == DOWN)
    verticalMotor.toggleDirection();      
  verticalMotor.setTargetDelay(verticalArmSpeedDelay);
  verticalMotor.doDistanceInCm(distance);
}

void VisionArm::clawRelease()
{
  claw.write(140);
}

void VisionArm::clawGrab()
{
  claw.write(35);
}

boolean VisionArm::isStopped()
{
  return horizontalMotor.isOff() && verticalMotor.isOff();
}

void VisionArm::doLoop()
{
  horizontalMotor.doLoop();
  verticalMotor.doLoop();
}

void VisionArm::stopNow()
{
  horizontalMotor.stopNow();
  verticalMotor.stopNow();
}
