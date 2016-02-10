import processing.serial.*;     // import the Processing serial library
Serial myPort; // instance of the setial object

float moveX, speed, inByte; //variables for the 'player'

// poaching Shiffman's car object 
// how would you make multiple cars to avoid?
Car myCar;

void setup() {
  size(800, 600);
  background(0);
  moveX=0;
  myCar = new Car(color(200, 10, 180));

  println(Serial.list());
  myPort = new Serial(this, Serial.list()[2], 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  background(0);

  speed = map(inByte, 0, 1023, 0, 6);

  myCar.drive();
  myCar.update(speed, moveX);
  myCar.display();
  if (moveX != 0) moveX = 0;
}

void serialEvent(Serial myPort) {
  // read the serial buffer:
  String myString = myPort.readStringUntil('\n');
  // if you got any bytes other than the linefeed:
  myString = trim(myString);

  // split the string at the commas
  // and convert the sections into integers:
  int sensors[] = int(split(myString, ','));

  if (sensors.length > 1) {
    if (sensors[0] == 1) moveX = -3;
    if (sensors[1] == 1) moveX = 3;
    inByte = sensors[2];
  }
  // send a byte to ask for more data:
  myPort.write("A");
}