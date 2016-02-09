import processing.serial.*;     // import the Processing serial library
Serial myPort; // instance of the setial object

float moveX, speed; //variables for the 'player'

// poaching Shiffman's car object 
// how would you make multiple cars to avoid?
Car myCar;

void setup() {
  size(800, 600);
  background(0);
  moveX=0;
  myCar = new Car(color(200, 10, 180));
}

void draw() {
  background(0);

  speed = map(mouseY, 0, height, 0, 4);

  myCar.drive();
  myCar.update(speed, moveX);
  myCar.display();
  if (moveX != 0) moveX = 0;
}

void keyPressed() {
  if (key == CODED) {
    if (keyCode == LEFT) {
      moveX = -3;
    }
    if (keyCode == RIGHT) {
      moveX = 3;
    }
  }
}