const int sensorPin = A0; // constant for the input pin
const int ledPin = 9;  // constant for the output pin

int sensorVal = 0;  // variable to hold the value from the sensor

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // make the led pin an output
  Serial.begin(9600);  // start serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorVal = analogRead(sensorPin); // read the value of the sensor and store it in a variable
  sensorVal = map(sensorVal, 420, 900, 0, 255); // map the sensor's input to a useful range
  analogWrite(ledPin, sensorVal); // write the value to the LED
  delay(2); // pause to let the ADC settle
  Serial.println(sensorVal); // send the mapped value to the Serial monitor
}
