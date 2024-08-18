#include <Arduino.h>

// Sensor pins
const int leftSensorPin = D5;
const int rightSensorPin = D4;

// LED pins
const int leftRedLedPin = A2;
const int leftBlueLedPin = A3;
const int leftGreenLedPin = D10;

const int rightRedLedPin = D11;
const int rightBlueLedPin = A6;
const int rightGreenLedPin = A7;

void setup() {
    pinMode(leftSensorPin, INPUT);
    pinMode(rightSensorPin, INPUT);
    pinMode(leftRedLedPin, OUTPUT);
    pinMode(leftBlueLedPin, OUTPUT);
    pinMode(leftGreenLedPin, OUTPUT);
    pinMode(rightRedLedPin, OUTPUT);
    pinMode(rightBlueLedPin, OUTPUT);
    pinMode(rightGreenLedPin, OUTPUT);
    Serial.begin(9600);
}

int getTracking() {
    int left = digitalRead(leftSensorPin);
    int right = digitalRead(rightSensorPin);

    if (left == LOW && right == LOW) {
        return 0;
    } else if (left == LOW && right == HIGH) {
        return 1;
    } else if (left == HIGH && right == LOW) {
        return 10;
    } else if (left == HIGH && right == HIGH) {
        return 11;
    }
    return -1; // Fallback case
}

void loop() {
    int trackingState = getTracking();

    if (trackingState == 0) {
        Serial.println("Both sensors on a white reflective surface");
        digitalWrite(leftRedLedPin, LOW);
        digitalWrite(leftBlueLedPin, LOW);
        digitalWrite(leftGreenLedPin, HIGH);
        digitalWrite(rightRedLedPin, LOW);
        digitalWrite(rightBlueLedPin, LOW);
        digitalWrite(rightGreenLedPin, HIGH);

    } else if (trackingState == 1) {
        Serial.println("Right sensor triggered");
        digitalWrite(leftRedLedPin, LOW);
        digitalWrite(leftBlueLedPin, LOW);
        digitalWrite(leftGreenLedPin, LOW);
        digitalWrite(rightRedLedPin, HIGH);
        digitalWrite(rightBlueLedPin, LOW);
        digitalWrite(rightGreenLedPin, LOW);

    } else if (trackingState == 10) {
        Serial.println("Left sensor triggered");
        digitalWrite(leftRedLedPin, HIGH);
        digitalWrite(leftBlueLedPin, LOW);
        digitalWrite(leftGreenLedPin, LOW);
        digitalWrite(rightRedLedPin, LOW);
        digitalWrite(rightBlueLedPin, LOW);
        digitalWrite(rightGreenLedPin, LOW);

    } else if (trackingState == 11) {
        Serial.println("Line detected");
        digitalWrite(leftRedLedPin, HIGH);
        digitalWrite(leftBlueLedPin, LOW);
        digitalWrite(leftGreenLedPin, HIGH);
        digitalWrite(rightRedLedPin, HIGH);
        digitalWrite(rightBlueLedPin, LOW);
        digitalWrite(rightGreenLedPin, HIGH);
    }

    delay(100); // Adjust the delay as necessary
}