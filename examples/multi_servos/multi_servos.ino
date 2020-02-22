/*
 * multi_servo example
 * 
 * This sketch shows how to control multiple servos
 * Servos can be started while other servos are still moving
 * A moving servo can be commanded to change position or speed while still in motion.
 * Comma seperated Servo index, angle and speed values are sent using serial monitor
 *
 * Michael Margolis Feb 21 2020
 */
#include "SlowServo.h"

const int NBR_SERVOS = 6;

const byte SERVO_PINS[NBR_SERVOS] = {2,3,4,5,6,7};

SlowServo myservo[NBR_SERVOS];  // create servo object(s) to control a servo

void setup() {
  Serial.begin(9600);
  Serial.println("Starting servos");
  for (int i = 0; i < NBR_SERVOS; i++) {
    myservo[i].attach(SERVO_PINS[i]);
  }
  Serial.print(NBR_SERVOS);
  Serial.println(" ready");
  Serial.println("Enter S followed by servo nbr (0-"); Serial.print(NBR_SERVOS-1);
  Serial.println(", angle(0-180), speed (degrees per second)");
  Serial.println(" example: S0,45,20\n  moves first servo to 45 degrees at 20 degrees per second");
}

void loop() {
  SlowServo::service(); // must be called at least 20 times per second, faster is ok
  if (Serial.available()) {
    if( Serial.read() == 'S') {
       int index = Serial.parseInt();
       int angle = Serial.parseInt();
       int speed = Serial.parseInt();
       Serial.print("moving servo "); Serial.print(index);
       Serial.print(" to "); Serial.print(angle);
       Serial.print(" at "); Serial.print(speed); Serial.println(" degrees per second");
       myservo[index].moveTo(angle, speed);   
    }
  }
}

// call this function instead of delay to ensure repeated servicing of moving servos
void myDelay(unsigned long dur)
{
  unsigned long start = millis();
  while (millis() - start  < dur)
     SlowServo::service();
}