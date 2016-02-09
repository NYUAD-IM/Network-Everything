/*
 Controlling servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob

 modified by Dirichi and Nahil on 6 February 2016 for 2 servos and 2 potentiometers
*/

#include <Servo.h>

Servo myservo1;  // create first servo object to control first servo
Servo myservo2;  // create second servo object to control second servo

int potpin1 = 0;  // analog pin used to connect the first potentiometer
int potpin2 = 1;  // analog pin used to connect the second potentiometer
int val1;    // variable to read the value from the first analog pin
int val2;    // variable to read the value from the second analog pin

void setup() {
  Serial.begin(9600); // open a serial connection
  myservo1.attach(9);  // attaches the first servo on pin 9 to the first servo object
  myservo2.attach(10);  // attaches the second servo on pin 10 to the second servo object
}

void loop() {
  val1 = analogRead(potpin1);            // reads the value of the first potentiometer (value between 0 and 1023)
  val1 = map(val1, 0, 1023, 0, 180);     // scale it to use it with the first servo (value between 0 and 180)
  val2 = analogRead(potpin2);            // reads the value of the second potentiometer (value between 0 and 1023)
  val2 = map(val2, 0, 1023, 0, 180);    // scale it to use it with the second servo (value between 0 and 180)
  myservo1.write(val1);   // sets the first servo position according to the first scaled value
  myservo2.write(val2);   // sets the second servo position according to the second scaled value
  
  Serial.write(val1);           // send the angle of servo1
  Serial.write(val2);           // send the angle of servo2
  delay(15);                    // wait for the servos to get to their positions
}

