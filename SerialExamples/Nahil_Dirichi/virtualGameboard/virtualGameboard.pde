// rotates a virtual game board on the x & y axes
// expects a serial connection that receives 2 bytes 
// each between 0-180 indicating the amount of rotation

import processing.serial.*; // import the serial library

Serial mySerial; // create a serial object

float xAngle, yAngle, xIn, yIn; // declare a bunch of variables
float[] serialInArray = new float[2];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive

void setup() {
  size(800, 600, P3D);  // canvas size, in 3D!

  printArray(Serial.list()); // list the serial ports
  String portName = Serial.list()[2]; // string to hold the port name
  mySerial = new Serial(this, portName, 9600); // call the serial constructor

  rectMode(CENTER); // draw our rectangle from the center
}

void draw() {
  background(0); // update background color
  translate(width/2, height/2, 0); // translate the origin for proper rotation
  xAngle = map(xIn, 0, 180, 75, 15); // map the xpot value
  yAngle = map(yIn, 0, 180, -30, 30); // map the ypot value
  rotateX(radians(xAngle)); // rotate on the x axis
  rotateY(radians(yAngle)); // rotate on the y axis
  rect(0, 0, 300, 300); // draw the rectangle
}

// serialEvent fires when something comes into the serial port
// because i know the data coming in is going to be 2 bytes
// and i know th eorder of the bytes coming in
// i can read them sequentially into the proper spaces

void serialEvent(Serial mySerial) {
  // read a byte from the serial port:
  int inByte = mySerial.read();

  // Add the latest byte from the serial port to array:
  serialInArray[serialCount] = inByte;
  // increment the number of bytes we have received
  serialCount++;

  // If we have 2 bytes:
  if (serialCount > 1 ) {
    xIn = serialInArray[0]; // set the variable for x to the first item in the array
    yIn = serialInArray[1]; // set the variable for y to the second item in the array

    // print the values (for debugging purposes only):
    println("incoming  vals : " + xAngle + "\t" + yAngle);

    // Reset serialCount:
    serialCount = 0;
  }
}