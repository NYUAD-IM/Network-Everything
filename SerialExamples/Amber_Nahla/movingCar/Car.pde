class Car { 
  color c;
  float xPos;
  float yPos;
  float ySpeed;

  // The Constructor is defined with arguments.
  Car(color tempC) { 
    c = tempC;
    xPos = width/2;
    yPos = 40;
    ySpeed = 0;
  }

  void display() {
    stroke(0);
    fill(c);
    rectMode(CENTER);
    rect(xPos, yPos, 30, 60);
  }

  void update(float speed) {
    ySpeed = speed;
  }

  void drive(float xIncDec) {
    xPos = xPos+xIncDec;
    yPos = yPos + ySpeed;
    
    if (yPos > height+30) {
      yPos = -30;
    }
  }
}