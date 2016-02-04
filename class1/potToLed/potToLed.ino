const int potPin = A0; // constant for the input pin
const int ledPin = 9;  // constant for the output pin

int potVal = 0;  // variable to hold the value from the sensor


void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // make the led pin an output
  Serial.begin(9600);  // start serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  potVal = analogRead(potPin); // read the value of the pot and store it in a variable
  analogWrite(ledPin, potVal/4); // write the value to the LED, dividing by 4
  delay(2); // pause to let the ADC settle
  Serial.println(sensorVal); // send the mapped value to the Serial monitor
}
