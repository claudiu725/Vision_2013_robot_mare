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
#include "VisionDC.h"
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
elapsedMillis enemyTimer;
int baseStartState;

VisionState baseState, robotState;
VisionSensor enableSensor;
int colorRedStartState, colorYellowStartState, testStartState, onePointStartState, color;
boolean robotRunning;

void setup()
{
  Serial.begin(19200);
  base.init();
  //arm.init();
  enableSensor.initPin(enablePin);
  enableSensor.setAsPullup();
  ignoreSensors = true;//false;
  robotState = 0;
  baseState = STATE_STOP;

  colorRedStartState = 100;
  colorYellowStartState = STATE_STOP;
  testStartState = 300;
  onePointStartState = 200;
  color = YELLOW; // RED YELLOW TEST ONEPOINT
}

void serialEvent()
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    double x;
    switch (inChar) {
      case 'w':
        x = Serial.parseFloat();
        x = x==0 ? 10 : x;
        base.moveForward(x,mediumSpeedDelay);
        Serial.print("Move forward:");
        Serial.println(x);
        break;
      case 's':
        x = Serial.parseFloat();
        x = x==0 ? 10 : x;
        base.moveBackward(x,mediumSpeedDelay);
        Serial.print("Move backward:");
        Serial.println(x);
        break;
      case 'a':
        x = Serial.parseFloat();
        x = x==0 ? 90 : x;
        base.turnLeft(x);
        Serial.print("Turn left:");
        Serial.println(x);
        break;
      case 'd':
        x = Serial.parseFloat();
        x = x==0 ? 90 : x;
        base.turnRight(x);
        Serial.print("Turn right:");
        Serial.println(x);
        break;
      default:
        Serial.print(inChar);
    }
  }
}

void loop()
{
  switch (robotState)
  {
    case 0:
      robotRunning = true;//false;
      robotState.waitFor(enableSensorDetect, STATE_NEXT);
      break;
    case 1:
      switch (color)
      {
        case RED:
          baseState.wait(5000,colorRedStartState);
          break;
        case YELLOW:
          baseState.wait(5000,colorYellowStartState);
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
      base.moveForward(39,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:
      base.moveBackward(27,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:
      base.frontFront.disable();
      base.moveForward(23,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 5:
      base.frontFront.enable();
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;

    default:
      baseState.doLoop();
  }

  //*************************************************************************//
  if (robotRunning)
  {
    base.checkObstructions();
    if (!base.isStopped())
    {
      if (base.obstructionDetected == true && ignoreSensors == false)
      {
        base.pause();
        enemyTimer = 0;
      }
      else if (enemyTimer > 3000)
        base.unpause();
    }

    base.doLoop();
    arm.doLoop();
  }
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
  return 0;//arm.verticalMotor.isOff();
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
  baseState = STATE_STOP;
}
