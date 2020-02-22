/*
  SlowServo.h - Software driven servo control using service routine polling
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

#include <Stdint.h>

const int  SERVO_FRAME_INTERVAL  = 20; // ms per frame
const int  SERVO_FRAME_FREQUENCY = 1000 / SERVO_FRAME_INTERVAL; 
//const int  MAX_SERVO_SPEED = 360;     // set this for your servo
const int  POS_NOT_KNOWN = 255;       // positon of servo not yet know
const int  MAX_SOFT_SERVOS  = 6;
   
class SlowServo
{
   private:
    uint8_t thisInstance;

    static uint8_t nbrServos;
    static uint8_t pins[MAX_SOFT_SERVOS];
    static uint16_t currentPos[MAX_SOFT_SERVOS];
    static uint16_t targetPos[MAX_SOFT_SERVOS];
    static int speed[MAX_SOFT_SERVOS]; // degrees per second  
    static int8_t offset[MAX_SOFT_SERVOS];  // +- 127 degrees
    static bool isReversed[MAX_SOFT_SERVOS];
    static void write(uint8_t index, uint16_t pw);
    static uint16_t angleToPW(uint8_t angle) ;
   
    
   public:
    SlowServo();    
    void attach(uint8_t pin);  
    void detach();       
    // move the servo at the fastest speed
    void moveTo(uint8_t angle);
    // move at the given degrees per second
    void moveTo(uint8_t angle, int dps);
    void setReverse(bool isReversed);  // default is not reveresed  
    void setOffset(int offset);     
    bool isMoving(); // true if this instance is moving
    static bool isIdle();  // true if all instances are idle
    static void service(); // must be called at least once per SERVO_FRAME_INTERVAL  
};
