#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include <Wire.h>
#include <ADJDS311.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"
#include "VisionBase.h"
#include "VisionArm.h"
#include "VisionState.h"
#include "VisionBrushless.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define NINETYSECONDS 88000L

#define RED 0
#define YELLOW 1
#define TEST 2
#define ONEPOINT 3

VisionBase base;
VisionArm arm;
boolean ignoreSensors = false;
int baseStartState;

VisionState baseState, armState, robotState, clawState;
VisionSensor enableSensor;
int colorRedStartState, colorYellowStartState, testStartState, onePointStartState, color;
boolean fruitWasDetected;
boolean robotRunning;

void setup()
{
  base.init();
  arm.init();
  enableSensor.initPin(enablePin);
  enableSensor.setAsPullup();
  ignoreSensors = true;

  robotState = 0;
  baseState = STATE_STOP;
  armState.waitFor(armVerticalStop, STATE_STOP);
  clawState = STATE_STOP;
  
  colorRedStartState = 100;
  colorYellowStartState = 0;
  testStartState = 50;
  onePointStartState = STATE_STOP;
  color = TEST; // RED YELLOW TEST ONEPOINT
}

#define RETRIEVE_A 60
#define RETRIEVE_B 100

int retrieveOption;

void loop()
{
  switch (robotState)
  {
    case 0:
      robotRunning = false;
      robotState.waitFor(enableSensorDetect, STATE_NEXT);
      break;
    case 1:
      switch (color)
      {
        case RED:
          baseState = colorRedStartState;
          break;
        case YELLOW:
          baseState = colorYellowStartState;
          break;
        case TEST:
          baseState = testStartState;
          break;
        case ONEPOINT:
          baseState = onePointStartState;
          break;
      }
      robotRunning = true;
      robotState.wait(NINETYSECONDS, STATE_NEXT);
      break;
    case 2:
      robotRunning = false;
      timeIsUpStopEverything();
      robotState = STATE_STOP;
      break;
    default:
      robotState.doLoop();
      break;
  }

  switch (baseState)
  {
    case 0:
      base.moveForward(36,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:
      base.moveBackward(13,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:
      base.moveForward(60,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 5: //PRIMUL FRUCT
      armState = 0;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 6:
      base.moveForward(7,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 7: //AL DOILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 8:  
      base.moveForward(10, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 9: //AL TREILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 10:
      base.moveForward(27 , mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 11:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 12:
      base.moveForward(22, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 13: //primul fruct - POMUL 2
      armState = 0;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 14:
      base.moveForward(7,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 15: //AL DOILEA FRUCT - POMUL 2
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 16:  
      base.moveForward(10, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 17: //AL TREILEA FRUCT - POMUL 2
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 18:
      base.moveForward(174, fastSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 19:
      base.turnRight(91);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 20:
      base.moveForward(128, fastSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 21:
      base.turnRight(90);  
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 22:
      baseState = STATE_STOP;
      arm.basketOpen();
      break;
      
    case 50:
      base.moveForward(50, veryFastSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 51:
      base.moveBackward(50, veryFastSpeedDelay);
      baseState.waitFor(baseStop, 50);
      break;

    case 100:
      base.moveForward(39,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 101:
      base.turnRight(90+90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 102:
      arm.moveHorizontal(10, FORWARD);
      baseState.waitFor(armStop, STATE_NEXT);
      break;
    case 103:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 104:
      arm.moveHorizontal(17, BACKWARD);
      baseState.waitFor(armStop, STATE_NEXT);
      break;
    case 105:
      base.moveBackward(17,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 106:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 107:
      base.moveForward(100,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 108:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 109:
      base.moveBackward(35,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 110: //PRIMUL FRUCT
      armState = 0;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 111:
      base.moveForward(7,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 112: //AL DOILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 113:  
      base.moveForward(10, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 114: //AL TREILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 115:
      base.moveForward(27 , mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 116:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 117:
      base.moveForward(22, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 118: //primul fruct - POMUL 2
      armState = 0;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 119:
      base.moveForward(7,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 120: //AL DOILEA FRUCT - POMUL 2
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 121:  
      base.moveForward(10, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 122: //AL TREILEA FRUCT - POMUL 2
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 123:
      base.moveForward(174, fastSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 124:
      base.turnRight(91);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 125:
      base.moveForward(128, fastSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 126:
      base.turnRight(90);  
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 127:
      baseState = STATE_STOP;
      arm.basketOpen();
      break;

    default:
      baseState.doLoop();
  }
  
  //*************************************************************************//
  switch (armState)
  {
    // arm open routine
    case 0:
      arm.clawRelease();
      arm.moveHorizontal(17, FORWARD);
      armState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 1:
      retrieveOption = RETRIEVE_B;
      clawState = 0;
      armState.save();
      armState = STATE_STOP;
      break;
    case 2:
      baseState.restore();
      armState = STATE_STOP;
      break;
      
    case 10:
      arm.clawRelease();
      arm.moveHorizontal(7, FORWARD);
      armState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 11:
      retrieveOption = RETRIEVE_A;
      clawState = 0;
      armState.save();
      armState = STATE_STOP;
      break;
    case 12:
      baseState.restore();
      armState = STATE_STOP;
      break;
    default:
      armState.doLoop();
  }
  
  switch (clawState)
  {
    //arm is already extended, check mid and low, stay lowered
    case 0:
      arm.moveVertical(arm.sensorBottom);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 1:
      if (fruitDetect())
        clawState.call(20);
      else
        clawState = STATE_NEXT;
      break;
    case 2:
      arm.moveVertical(arm.sensorMiddle);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 3:
      //if (fruitDetect())
        clawState.call(20);
      //else
        //clawState = STATE_NEXT;
      break;
    case 4:
      clawState = 40;//didn't find it
      break;

    //found something, grab it and check it to be purple
    case 20:
      arm.clawGrab();
      clawState.wait(500, STATE_NEXT);
      break;
    case 21:
      if (seePurple() || seeClear())
        clawState = retrieveOption; //got it
      else
      {
        arm.clawRelease();
        clawState.restore(); //return to case 0+
      }
      break;

    //failure, did not detect anything
    case 40:
      arm.moveVertical(arm.sensorTop);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 41:
      clawState = 80; //done
      break;      

    //found something, claw grabbed fruit and checked it to be purple
    //now take it up, pull it back and drop it in the cup
    // RETRIEVE FRUIT A
    case 60:
      arm.moveVertical(arm.sensorTop);
      clawState.waitFor(armVerticalStop, 62);
      break;
    case 62:
      arm.moveHorizontal(8, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 63:
      base.turnLeft(45);
      clawState.waitFor(baseStop, 65);
      break;
    case 65:
      arm.clawRelease();
      clawState.wait(1000, STATE_NEXT);
      break;
    case 66:
      arm.moveHorizontal(17, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 67:
      base.turnRight(45);
      clawState.waitFor(baseStop, STATE_NEXT);
      break;
    case 68:
      clawState = 81;
      break;
      
    //done
    case 80:
      arm.moveHorizontal(17, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 81:
      armState.restore();
      clawState = STATE_STOP;
      break;

    // RETRIEVE FRUIT B    
    case 100:
      arm.moveVertical(arm.sensorTop);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 101:
      arm.moveHorizontal(10, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 102:
      base.turnLeft(90);
      clawState.waitFor(baseStop,104);
      break;
      /*
    case 103:
      arm.moveHorizontal(7, FORWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
      */
    case 104:
      arm.clawRelease();
      arm.moveHorizontal(17, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 105:
      base.turnRight(90);
      clawState.waitFor(baseStop,STATE_NEXT);
      break;
    case 106:
      clawState = 81;
      break;

    default:
      clawState.doLoop();
  }

  //*************************************************************************//
  if (robotRunning)
  {
    base.checkObstructions();
    if (!base.isStopped())
    {
      if (base.obstructionDetected == true && ignoreSensors == false)
        base.pause();
      else
        base.unpause();
    }
    if (arm.fruitBarrier.detect())
      fruitWasDetected = true;

    if (arm.horizontalLimiter.detect() && !arm.horizontalMotor.isOff() && arm.horizontalDirection == BACKWARD)
      arm.horizontalMotor.setRemainingDistance(0.05);
    if (arm.horizontalAntiSlip.detect() && arm.horizontalMotor.isOff())
      arm.moveHorizontal(1, BACKWARD);

    base.doLoop();
    arm.doLoop();
  }
}

boolean seePurple()
{
  return arm.fruitColor.isPurple();
}

boolean seeBlack()
{
  return arm.fruitColor.isBlack();
}

boolean seeClear()
{
  return arm.fruitColor.isClear();
}

boolean baseStop()
{
  return base.isStopped();
}

boolean armStop()
{
  return arm.isStopped();
}

boolean armVerticalStop()
{
  return arm.verticalMotor.isOff();
}

boolean armHorizontalStop()
{
  return arm.horizontalMotor.isOff();
}

boolean armHorizontalStopOrFruit()
{
  return arm.horizontalMotor.isOff() || arm.fruitBarrier.detect();
}

boolean fruitDetect()
{
  return arm.fruitBarrier.detect();
}

boolean verticalLimiterTrigger()
{
  return arm.verticalLimiter.detect();
}

boolean horizontalLimiterTrigger()
{
  return arm.horizontalLimiter.detect();
}

boolean horizontalAntiSlipTrigger()
{
  return arm.horizontalAntiSlip.detect();
}

boolean horizontalSensor()
{
  return arm.horizontalAntiSlip.detect() || arm.horizontalLimiter.detect();
}

boolean enableSensorDetect()
{
  return enableSensor.detect();
}

void timeIsUpStopEverything()
{
  base.stopNow();
  arm.stopNow();
  base.disable();
  arm.disable();
  baseState = STATE_STOP;
  armState = STATE_STOP;
  clawState = STATE_STOP;
}
