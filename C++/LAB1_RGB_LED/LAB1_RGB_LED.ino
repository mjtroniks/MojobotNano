// Define RGB LED pins
const int redPin = A2;    // Red LED connected to digital pin 11
const int greenPin = 10;  // Green LED connected to digital pin 10
const int bluePin = A3;    // Blue LED connected to digital pin 9

// the setup function runs once when you press reset or power the board
void setup() {
  // Set RGB LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

// Helper function to set color
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

// the loop function runs over and over again forever
void loop() {
  setColor(255, 0, 0);  // Red
  delay(1000);
  setColor(0, 255, 0);  // Green
  delay(1000);
  setColor(0, 0, 255);  // Blue
  delay(1000);
  
}