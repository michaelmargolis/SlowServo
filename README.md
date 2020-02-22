# SlowServo #

A library to move servos from one degree per second up to the maximum speed of the servo used.
Up to six servos are supported although more can be added by changing a define in the library.

### Usage is similar to the Servo library with the following differences ###
* Up to six SlowServo instances can be created (you can change MAX_SOFT_SERVOS = 6 to a higher number in SlowServo.cpp but this consume a little more memory)
* the SlowServo::service() must be called at least 20 times per second, faster is ok. If you need to delay, use code similar to the myDelay function shown in the multi_servos example sketch.
* the attach method does not accept arguments for min and max pulse width
* readMicroseconds and writeMicroseconds are not supported

### Examples ###
* Slow_servo.ino controls a single servo using commands sent from the serial monitor. Usage is described in the sketch code. In this sketch,  code execution halts until the servo has completed movement. See the next example for code that does not block
* Multi_servos.ino controls up to six servos using commands from the serial monitor.   Servos can be started while other servos are still moving.  A moving servo can be commanded to change position or speed while still in motion. See sketch code for usage.


* If you need help installing the library, see: https://www.arduino.cc/en/guide/libraries  Importing a .zip Library