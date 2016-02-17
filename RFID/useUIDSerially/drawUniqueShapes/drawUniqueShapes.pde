import processing.serial.*;

Serial myPort;        // The serial port
// the value read from the microcontroller
// we need to give the string an initial value 
// so as to not get a null pointer error
String inString = "000000"; 

void setup () {
  // set the window size:
  size(400, 300);

  // check your port below
  // println(Serial.list());

  myPort = new Serial(this, Serial.list()[4], 115200);

  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
}
void draw () {
  background(255);

  if (inString.equals("621153299")) {
    ellipse(width/2, height/2, 100, 100);
  } else if (inString.equals("28321276")) {
    rectMode(CENTER);
    rect(width/2, height/2, 100, 100);
  } else if (inString.equals("3442456957")) {
    triangle(width/2, 100, width/4, 200, (width/4)*3, 200);
  } else {
    bezier(85, 20, 10, 10, 90, 90, 15, 80);
  }
}

void serialEvent (Serial myPort) {
  
  println("I got some information!");
  inString = myPort.readStringUntil('\n'); // read the buffer

  // is it valid?
  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
    println(inString);
  }
}