int ledPin = 2; // variable to hold the value of the pin the LED is attached to
int switchPin = 3; // variable to hold the pin the button is attached to
int switchState; // variable to hold the value from the buton

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // set the led pin an output
  pinMode(switchPin, INPUT); // set the button pin as an input
}

void loop() {
  // put your main code here, to run repeatedly:
  switchState = digitalRead(switchPin); // read the voltage on the button pin
  // if the button is pressed
  if (switchState == HIGH) { 
    // blink the LED
    digitalWrite(ledPin, LOW);
    delay(50);
    digitalWrite(ledPin, HIGH);
    delay(50);
    // if the button is not pressed
  } else {
    // turn the LED off
    digitalWrite(ledPin, LOW);
  }
}
