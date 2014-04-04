#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"
#include "VisionSensorsArm.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define STATE_STOP -1
#define STATE_WAIT -2
#define STATE_WAIT_MOTORS_STOP -3

elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait;
VisionStepper motorLeft;
VisionStepper motorRight;
sensors_and_arm SnA;

int state;

void setup()
{
  //Serial.begin(9600);
  //Serial.println("setup
  SnA.init();
  
  motorLeft.init();
  motorLeft.initPins(enablePinLeft, directionPinLeft, stepPinLeft);
  motorLeft.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorLeft.initSizes(wheelDiameter, wheelRevolutionSteps);
  
  motorRight.init();
  motorRight.initPins(enablePinRight, directionPinRight, stepPinRight);
  motorRight.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorRight.initSizes(wheelDiameter, wheelRevolutionSteps);
  
  pinMode(buttonTestPin, INPUT_PULLUP);
  delay(1000);
  state = 0;
}

void loop()
{ 
  switch (state)
  {
    case 0:
      //move forward
      MoveForward(50.0,4000);
      waitForMotorsStop(state + 1);
      break;
    case 1:
      TurnRight(180);
      waitForMotorsStop(state + 1);  
    case 2:
      
      break;
    case 3:
    
      break;
    case 4:
    
      break;
    case STATE_STOP:   //stop
      break;
    case STATE_WAIT:
      if (wait_time > time_to_wait)
      {
        state = state_to_set_after_wait;
      }
      break;
    case STATE_WAIT_MOTORS_STOP:
      if (motorLeft.isOff() && motorRight.isOff())
      {
        state = state_to_set_after_wait;
      }
      break;
  }
  if (SnA.detectFront() || SnA.detectBack() || SnA.detectLeft() || SnA.detectRight())
  {
    motorLeft.pause();
    motorRight.pause();
  }
  else
  {
    motorLeft.unpause();
    motorRight.unpause();
  }
  motorLeft.doLoop();
  motorRight.doLoop();
}

void wait(int time_in_ms, int state_after)
{
  state = STATE_WAIT;
  wait_time = 0;
  time_to_wait = time_in_ms;
  state_to_set_after_wait = state_after;
}

void waitForMotorsStop(int state_after)
{
  state = STATE_WAIT_MOTORS_STOP;
  state_to_set_after_wait = state_after;
}

void MoveForward(float distance, int step_delay)
{       
  float counter = 0;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();  
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void MoveBackward(float distance, int step_delay)
{       
  float counter = 0;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.toggleDirection();    
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void TurnLeft(int angle)
{      
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();
  motorLeft.toggleDirection();        
  motorLeft.doSteps(wheelDiameter * 10 / 90 *angle);
  motorRight.doSteps(wheelDiameter * 10 / 90 *angle);
  /*
  motorLeft.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  motorRight.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  */  
}

void TurnRight(int angle)
{  
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.doSteps(wheelDiameter * 10 / 90 *angle);
  motorRight.doSteps(wheelDiameter * 10 / 90 *angle);
  /*
  motorLeft.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  motorRight.doDistanceInCm(2 * PI * distanceBetweenWheels / 360 * angle);
  */  
}
