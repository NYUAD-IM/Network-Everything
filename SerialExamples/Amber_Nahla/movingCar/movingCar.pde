import processing.serial.*;     // import the Processing serial library
Serial myPort; // instance of the setial object

float moveX, speed; //variables for the 'player'
float inByte; // variable for the car speed

// poaching Shiffman's car class 
// how would you make multiple cars to avoid?
Car myCar;

void setup() {
  size(800, 600);
  background(0);
  moveX=0; // initialize horizontal movement variable
  myCar = new Car(color(200, 10, 180)); // new instance of the car class

  println(Serial.list()); // print out serial ports
  myPort = new Serial(this, Serial.list()[2], 9600); // create an object
  myPort.bufferUntil('\n'); // wait until we receive a newline character
}

void draw() {
  background(0); // erase bg

  speed = map(inByte, 0, 1023, 0, 6); // map the sensor byte to the speed of the car

  myCar.drive(moveX); // update car's position
  myCar.update(speed); // update the speed 
  myCar.display(); // draw the car
  if (moveX != 0) moveX = 0; // reset the move var to 0 f necessary
}

// only triggers once newline char is received
void serialEvent(Serial myPort) {
  // read the serial buffer:
  String myString = myPort.readStringUntil('\n');
  // if you got any bytes other than the linefeed:
  myString = trim(myString);

  // split the string at the commas
  // and convert the sections into integers:
  int sensors[] = int(split(myString, ','));

  // as long as we have valid data
  if (sensors.length > 1) {
    if (sensors[0] == 1) moveX = -3; // set the move variable
    if (sensors[1] == 1) moveX = 3; // set the move variable
    inByte = sensors[2];  // set the speed variable
  }
  // send a byte to ask for more data:
  myPort.write("A");
}