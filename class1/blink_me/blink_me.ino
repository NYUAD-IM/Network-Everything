int ledPin = 2; // variable to hold the LED pin

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // set the led pin an output
}

void loop() {
  // put your main code here, to run repeatedly:
  // turn the LED on
  digitalWrite(ledPin, HIGH);
  delay(250); // pause for a moment 
  digitalWrite(ledPin, LOW); // turn the LED off
  delay(1500);  // pause for a longer moment
}
