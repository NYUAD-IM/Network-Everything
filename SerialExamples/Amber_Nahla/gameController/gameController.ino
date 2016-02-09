// code for the RGB LED is adapted from sparkfun website

/*  circuit has one momentary switch attched to pin 2
 *  and one momentary switch attched to pin 7
 *  a soft Pot is attached to A0  */

// L for left and R for right

int ledPinL = 13;
int switchPinL = 2;
int ledPinR = 12;
int switchPinR = 7;
int switchStateL;
int switchStateR;

int RED_LED_PIN = 9;    // Red LED Pin
int GREEN_LED_PIN = 10; // Green LED Pin
int BLUE_LED_PIN = 11;  // Blue LED Pin

int SENSOR_PIN = 0;      // Analog input pin
int redValue, greenValue, blueValue;

void setup() {

  pinMode(ledPinL, OUTPUT);
  pinMode(switchPinL, INPUT);
  pinMode(ledPinR, OUTPUT);
  pinMode(switchPinR, INPUT);

  Serial.begin(9600); // start the serial connection

  establishContact(); // call this function to make sure you're connected
}

void loop() {

  switchStateL = digitalRead(switchPinL);
  switchStateR = digitalRead(switchPinR);

  //turning on the LED when switch button is pressed

  if (switchStateL == HIGH) {
    digitalWrite(ledPinL, HIGH);
  } else {
    digitalWrite(ledPinL, LOW);
  }

  if (switchStateR == HIGH) {
    digitalWrite(ledPinR, HIGH);
  } else {
    digitalWrite(ledPinR, LOW);
  }

  int sensorValue;

  sensorValue = analogRead(SENSOR_PIN);

  setRGB(sensorValue);

// here's where you send serial information
// wait for a byte to arrive from the connected
// device, then send the values
  if (Serial.available() > 0) {
    // let's send all the values!
    // it'll be awesome
    Serial.print(switchStateL); // switch 1
    Serial.print(','); // delimiter
    Serial.print(switchStateR); // switch 2
    Serial.print(','); // delimiter
    Serial.println(sensorValue); // sensor + NL/CR
  }
}

// this function runs until a connected device sends a byte
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}

void setRGB(int RGBposition) {
  int mapRGB1, mapRGB2, constrained1, constrained2;

  // Here we take RGBposition and turn it into three RGB values.

  // The three values are computed so that the colors mix and
  // produce a rainbow of colors across the 0-1023 input range.

  // For each channel (red green blue), we're creating a "peak"
  // a third of the way along the 0-1023 range. By overlapping
  // these peaks with each other, the colors are mixed together.
  // This is most easily shown with a diagram:
  // http://sfecdn.s3.amazonaws.com/education/SIK/SchematicImages/Misc/RGB_function.jpg

  // Create the red peak, which is centered at 0.
  // (Because it's centered at 0, half is after 0, and half
  // is before 1023):

  mapRGB1 = map(RGBposition, 0, 341, 255, 0);
  constrained1 = constrain(mapRGB1, 0, 255);

  mapRGB2 = map(RGBposition, 682, 1023, 0, 255);
  constrained2 = constrain(mapRGB2, 0, 255);

  redValue = constrained1 + constrained2;

  // Create the green peak, which is centered at 341
  // (one-third of the way to 1023):

  // Note that we've nested the functions by putting the map()
  // function inside the constrain() function. This can make your
  // code more compact, and requires fewer variabls:

  greenValue = constrain(map(RGBposition, 0, 341, 0, 255), 0, 255)
               - constrain(map(RGBposition, 341, 682, 0, 255), 0, 255);

  // Create the blue peak, which is centered at 682
  // (two-thirds of the way to 1023):

  blueValue = constrain(map(RGBposition, 341, 682, 0, 255), 0, 255)
              - constrain(map(RGBposition, 682, 1023, 0, 255), 0, 255);

  // Now we have all three brightnesses,
  // we just need to display the computed color:

  analogWrite(RED_LED_PIN, redValue);
  analogWrite(GREEN_LED_PIN, greenValue);
  analogWrite(BLUE_LED_PIN, blueValue);

}
