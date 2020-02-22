/*
   slow_servo example

   This sketch shows how to move a servo at slow speed.
   Comma seperated angle and speed values are sent using serial monitor 
   
   Michael Margolis Feb 21 2020
*/
#include "SlowServo.h"

const byte SERVO_PIN = 2;

SlowServo myservo;  // create servo object(s) to control a servo


void setup() {
  Serial.begin(9600);
  myservo.attach(SERVO_PIN);

  Serial.println("Enter A followed by angle(0-180), speed (degrees per second)");
  Serial.println(" example: A45,20\n  moves servo to 45 degrees at 20 degrees per second");
}

void loop() {
  SlowServo::service(); // must be called at least 20 times per second, faster is ok
  if (Serial.available()) {
    if ( Serial.read() == 'A') {
      int angle = Serial.parseInt();
      int speed = Serial.parseInt();
      Serial.print("moving servo to "); Serial.print(angle);
      Serial.print(" at "); Serial.print(speed); Serial.println(" degrees per second");

      myservo.moveTo(angle, speed);
      // following loop waits until servo has reached commanded angle
      // see multi_servos.ino example to see how to move servos while running other code
      while (myservo.isMoving()) {
        SlowServo::service();  // repeatedly call this method while servo is moving
      }
    }
  }
}