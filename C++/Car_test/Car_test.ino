#include <Arduino.h>


// Pin configuration
const int triggerPin = 2;
const int echoPin = 3;

const int ledLeftRed = A2;
const int ledLeftGreen = 10;
const int ledLeftBlue = A3;

const int ledRightRed = 11;
const int ledRightGreen = A7;
const int ledRightBlue = A6;

const int infraredLeft = 5;
const int infraredRight = 4;

const int motor1PWM = 7;
const int motor1Dir = 8;
const int motor2PWM = 6;
const int motor2Dir = 9;




void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledLeftRed, OUTPUT);
  pinMode(ledLeftGreen, OUTPUT);
  pinMode(ledLeftBlue, OUTPUT);

  pinMode(ledRightRed, OUTPUT);
  pinMode(ledRightGreen, OUTPUT);

  pinMode(infraredLeft, INPUT);
  pinMode(infraredRight, INPUT);

  pinMode(motor1PWM, OUTPUT);
  pinMode(motor1Dir, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  pinMode(motor2Dir, OUTPUT);

  Serial.begin(9600); // Initialize serial communication for debugging
  Serial.println("Setup complete.");


}

long measureDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration * 0.0343) / 2;

  Serial.print("Measured distance: ");
  Serial.println(distance);

  return distance;
}

void controlRedLEDsByDistance() {
  Serial.println("Starting controlRedLEDsByDistance");

  unsigned long startTime = millis();
  while (millis() - startTime < 5000) { // Stay in this loop for 5 seconds
    long distance = measureDistance();
    int blinkDelay;

    if (distance <= 5) {
      blinkDelay = 100;
    } else if (distance >= 40) {
      blinkDelay = 1000;
    } else {
      blinkDelay = map(distance, 5, 40, 100, 1000);
    }

    digitalWrite(ledLeftRed, HIGH);
    digitalWrite(ledRightRed, HIGH);

    delay(blinkDelay);
    digitalWrite(ledLeftRed, LOW);
    digitalWrite(ledRightRed, LOW);
    delay(blinkDelay);
  }

  Serial.println("Finished controlRedLEDsByDistance");
}

void controlBlueLEDsByInfrared() {
  Serial.println("Starting controlBlueLEDsByInfrared");

  unsigned long startTime = millis();
  while (millis() - startTime < 5000) { // Stay in this loop for 5 seconds
    int leftValue = digitalRead(infraredLeft);
    int rightValue = digitalRead(infraredRight);

    if (leftValue == LOW) {
      digitalWrite(ledLeftRed, HIGH);
      delay(100);
      digitalWrite(ledLeftRed, LOW);
      delay(100);
    } else {
      digitalWrite(ledLeftRed, HIGH);
    }

    if (rightValue == LOW) {
       digitalWrite(ledRightRed, HIGH);
       delay(100);
      digitalWrite(ledRightRed, LOW);
      delay(100);
    } else {
       digitalWrite(ledRightRed, HIGH);
    }
    
  }

 
  Serial.println("Finished controlBlueLEDsByInfrared");
}

void controlMotorsAndGreenLEDs() {
  Serial.println("Starting controlMotorsAndGreenLEDs");

  auto mapDistanceToBlinkDelay = [](long distance) -> int {
    if (distance <= 2) {
      return 100;
    } else if (distance >= 50) {
      return 1000;
    } else {
      return map(distance, 2, 50, 100, 1000);
    }
  };

  unsigned long startTime = millis();

  // Test left motor and left green LED
  digitalWrite(motor1Dir, HIGH);
  analogWrite(motor2PWM, 0); // Ensure right motor is off
  while (millis() - startTime < 5000) { // Stay in this loop for 5 seconds
    long distance = measureDistance();
    int blinkDelay = mapDistanceToBlinkDelay(distance);

    digitalWrite(ledLeftGreen, HIGH);
    analogWrite(motor1PWM, 255);

    Serial.print("Distance (Left Motor): ");
    Serial.print(distance);
    Serial.print(" - Blink Delay: ");
    Serial.println(blinkDelay);

    delay(blinkDelay);
    digitalWrite(ledLeftGreen, LOW);
    analogWrite(motor1PWM, 0);
    delay(blinkDelay);
  }
  analogWrite(motor1PWM, 0);

  delay(5000);

  startTime = millis(); // Reset start time for the next sequence

  // Test right motor and right green LED
  digitalWrite(motor2Dir, HIGH);
  analogWrite(motor1PWM, 0); // Ensure left motor is off
  while (millis() - startTime < 5000) { // Stay in this loop for 5 seconds
    long distance = measureDistance();
    int blinkDelay = mapDistanceToBlinkDelay(distance);

    digitalWrite(ledRightGreen, HIGH);
    analogWrite(motor2PWM, 255);

    Serial.print("Distance (Right Motor): ");
    Serial.print(distance);
    Serial.print(" - Blink Delay: ");
    Serial.println(blinkDelay);

    delay(blinkDelay);
    digitalWrite(ledRightGreen, LOW);
    analogWrite(motor2PWM, 0);
    delay(blinkDelay);
  }
  analogWrite(motor2PWM, 0);
  delay(5000);

  Serial.println("Finished controlMotorsAndGreenLEDs");
}

void testRobot() {
  Serial.println("Starting testRobot");

  controlRedLEDsByDistance();
  controlBlueLEDsByInfrared();
  controlMotorsAndGreenLEDs();

  Serial.println("Finished testRobot");
}

void loop() {
  testRobot();
  delay(5000); // Adding a delay to avoid constant looping and cluttering the serial monitor
}
