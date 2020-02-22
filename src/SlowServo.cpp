/*
  SlowServo.cpp - Software driven servo control using service routine polling
  Copyright (c) 2020 Michael Margolis.  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "SlowServo.h"
#include <Arduino.h>
#include <Servo.h>


uint8_t SlowServo::nbrServos = 0;
uint16_t SlowServo::currentPos[MAX_SOFT_SERVOS] = {0};
uint16_t SlowServo::targetPos[MAX_SOFT_SERVOS] = {0};
int SlowServo::speed[MAX_SOFT_SERVOS] = {0}; // degrees per second
bool SlowServo::isReversed[MAX_SOFT_SERVOS] = {false};
int8_t SlowServo::offset[MAX_SOFT_SERVOS] = {0}; // offset is in microseconds


Servo _myServos[MAX_SOFT_SERVOS];

// uncomment the following for debug info
//#define DEBUG_PRINTF
#ifdef DEBUG_PRINTF
#include "HardwareSerial.h" 
static char _buf[128];
#define mprintf(...)             \
  do {              \
    sprintf(_buf, __VA_ARGS__); Serial.write(_buf); \
  } while (0)
#else
#define mprintf(...)  
#endif

const uint16_t usPerDegree = (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) / 180;
 
SlowServo::SlowServo() {
  offset[thisInstance] = 0;
}

// NOTE - this version does not support min and max arguments 
void  SlowServo::attach(uint8_t pin) {
  if ( nbrServos < MAX_SOFT_SERVOS) {
    thisInstance =  nbrServos++;
    _myServos[thisInstance].attach(pin);
    mprintf("Attaching servo %d to pin %d\n", thisInstance, pin);
    moveTo(90); // default position
  }
}

void  SlowServo::detach() {
  if ( nbrServos < MAX_SOFT_SERVOS) {
    currentPos[thisInstance] = POS_NOT_KNOWN;
    targetPos[thisInstance]  = POS_NOT_KNOWN;
    _myServos[thisInstance].detach();
  }
}

// move the servo at the fastest speed
void  SlowServo::moveTo(uint8_t angle) {
  uint16_t pw = angleToPW(angle);
  write(thisInstance, pw);
  currentPos[thisInstance] =  targetPos[thisInstance] = pw;
}

// move at the given degrees per second
void  SlowServo::moveTo(uint8_t angle, int dps) {
  // if (  currentPos[thisInstance] != POS_NOT_KNOWN)
  //   currentPos[thisInstance] =  targetPos[thisInstance]; // save the previous position  (no check is done to confirm the servo has had time to move!)
  targetPos[thisInstance] = angleToPW(angle);
  speed[thisInstance] = dps;
  mprintf("moveTo Servo %d angle=%d, dps=%d pw=%d\n", thisInstance, angle, dps, targetPos[thisInstance]);

}

void SlowServo::setReverse(bool _isReversed)
{
  isReversed[thisInstance] = _isReversed;
}

void SlowServo::setOffset(int _offset)
{
  offset[thisInstance] = _offset;
}

void SlowServo::write(uint8_t index, uint16_t pw)
{
  pw += offset[index];
  if (isReversed[index]){
    pw = map(pw, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, MAX_PULSE_WIDTH,  MIN_PULSE_WIDTH);
  }
  _myServos[index].writeMicroseconds(pw);
  // mprintf("writing %d microseconds to servo %d\n", pw, index);
}

bool SlowServo::isMoving()
{
  //mprintf("cur=%d, target=%d\n", currentPos[thisInstance], targetPos[thisInstance]);
  if ( currentPos[thisInstance] == POS_NOT_KNOWN || targetPos[thisInstance] == POS_NOT_KNOWN) {
    return false;
  }
  return currentPos[thisInstance] != targetPos[thisInstance];
}

// static method returns true if all servos idle
bool SlowServo::isIdle()
{
  bool isIdle = true;
  for (int i = 0; i < nbrServos; i++ ) {
    if (currentPos[i] != targetPos[i])
      isIdle = false;
     //mprintf( " servo %d is %s", i,  currentPos[i] != targetPos[i] ? "moving" : "idle");
  }
  //mprintf("\n");
  return isIdle;
}

void SlowServo::service() {  
  static uint32_t prevService;
  if ( millis() - prevService >= SERVO_FRAME_INTERVAL ) {
    prevService = millis();
    for (uint8_t i = 0; i < nbrServos; i++ ) {
      if ( targetPos[i] != POS_NOT_KNOWN) { // ignore servos not yet written to
        // mprintf("Servo %d at %d, target is %d\n", i, currentPos[i], targetPos[i]);
        if (currentPos[i] == POS_NOT_KNOWN) {
          write(i, targetPos[i]);
          currentPos[i] = targetPos[i];
          mprintf("setting servo %d current pos to target pos at %d\n", i, targetPos[i]);
        }
        else {
          // move from previous to new at commanded speed
          int pwDelta = max( ((speed[i]*usPerDegree) / SERVO_FRAME_FREQUENCY), 2); // 2 microseconds per frame is slowest speed
          if (targetPos[i] > currentPos[i]) {
            int prevPos = currentPos[i];
            currentPos[i] = min(targetPos[i], currentPos[i] + pwDelta);
            mprintf("> Servo %d moving from %d to %d, target is %d, offset = %d\n", i, prevPos, currentPos[i], targetPos[i],offset[i] );
          }
          else if (targetPos[i] < currentPos[i]) {
            int prevPos = currentPos[i];
            currentPos[i] = max(targetPos[i], currentPos[i] - pwDelta);
            mprintf("< Servo %d moving from %d to %d, target is %d, offset = %d\n", i, prevPos, currentPos[i], targetPos[i], offset[i]);
          }
          write(i, currentPos[i]);
        }
      }
    }
  }
}

uint16_t SlowServo::angleToPW(uint8_t angle) {
    uint16_t pw =  map(angle, 0, 180, MIN_PULSE_WIDTH,  MAX_PULSE_WIDTH);
    mprintf("angle %d is pw %d\n", angle, pw);
    return pw;
}

