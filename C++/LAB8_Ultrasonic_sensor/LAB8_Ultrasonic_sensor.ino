#include <Arduino.h>

// Ultrasonic sensor pins
const int triggerPin = 2;
const int echoPin = 3;

void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize the trigger pin as an output
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);

    // Initialize the echo pin as an input
    pinMode(echoPin, INPUT);
}

float measure_distance() {
    // Trigger pulse to start measurement
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Measure the pulse width on the echo pin
    long duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout (max range)

    // Calculate distance in centimeters
    float distance = duration * 0.034 / 2;  // Speed of sound = 0.034 cm/us
    return distance;
}

void loop() {
    float distance_cm = measure_distance();
    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");
    delay(100);  // Adjust sleep duration as needed
}