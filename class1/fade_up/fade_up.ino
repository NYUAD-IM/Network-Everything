const int ledPin = 9;  // constant for the output pin

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // make the led pin an output
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // loop from 0 to 255 
  for (int i = 0; i < 255; i++) {
    analogWrite(ledPin, i); // write the value of i to the pin
    delay(33); // delay so we can see the fade
  }
  // loop from 255 to 0 
  for (int i = 255; i > 0 ; i--) {
    analogWrite(ledPin, i); // write the value of i to the pin
    delay(33); // delay so we can see the fade
  }  
}
