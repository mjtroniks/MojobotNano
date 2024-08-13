// Define RGB LED pins for the left LED
const int leftRedPin = A2;
const int leftGreenPin = D10;
const int leftBluePin = A3;

// Define RGB LED pins for the right LED
const int rightRedPin = D11;
const int rightGreenPin = A7;
const int rightBluePin = A6;

void setup() {
  // Set RGB LED pins as outputs
  pinMode(leftRedPin, OUTPUT);
  pinMode(leftGreenPin, OUTPUT);
  pinMode(leftBluePin, OUTPUT);

  pinMode(rightRedPin, OUTPUT);
  pinMode(rightGreenPin, OUTPUT);
  pinMode(rightBluePin, OUTPUT);
}

void loop() {
  // Set both LEDs to Red
  digitalWrite(leftRedPin, HIGH);
  digitalWrite(rightRedPin, HIGH);
  digitalWrite(leftGreenPin, 0);
  digitalWrite(rightGreenPin, 0);
  digitalWrite(leftBluePin, 0);
  digitalWrite(rightBluePin, 0);
  delay(1000);

  // Set both LEDs to Green
  digitalWrite(leftRedPin, 0);
  digitalWrite(rightRedPin, 0);
  digitalWrite(leftGreenPin, HIGH);
  digitalWrite(rightGreenPin, HIGH);
  digitalWrite(leftBluePin, 0);
  digitalWrite(rightBluePin, 0);
  delay(1000);

  // Set both LEDs to Blue
  digitalWrite(leftRedPin, 0);
  digitalWrite(rightRedPin, 0);
  digitalWrite(leftGreenPin, 0);
  digitalWrite(rightGreenPin, 0);
  digitalWrite(leftBluePin, HIGH);
  digitalWrite(rightBluePin, HIGH);
  delay(1000);
}