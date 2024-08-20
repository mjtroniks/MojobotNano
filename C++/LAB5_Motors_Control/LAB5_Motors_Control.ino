#include <Arduino.h>

// Define motor pins
const int motor1_pwm_pin = D7;
const int motor1_dir_pin = D8;
const int motor2_pwm_pin = D6;
const int motor2_dir_pin = D9;

void setup() {
  // Initialize motor pins
  pinMode(motor1_pwm_pin, OUTPUT);
  pinMode(motor1_dir_pin, OUTPUT);
  pinMode(motor2_pwm_pin, OUTPUT);
  pinMode(motor2_dir_pin, OUTPUT);


}

void motorsSpeed(int left_wheel_speed, int right_wheel_speed) {
  // Set direction for Motor 1 based on speed
  if (left_wheel_speed > 0) {
    digitalWrite(motor1_dir_pin, HIGH); // Forward
  } else {
    digitalWrite(motor1_dir_pin, LOW);  // Backward
  }

  // Set direction for Motor 2 based on speed
  if (right_wheel_speed > 0) {
    digitalWrite(motor2_dir_pin, HIGH); // Forward
  } else {
    digitalWrite(motor2_dir_pin, LOW);  // Backward
  }

  // Convert speed to PWM duty cycle (0-255 for Arduino)
  left_wheel_speed = abs(left_wheel_speed); // Ensure positive value
  left_wheel_speed = constrain(left_wheel_speed, 0, 100); // Constrain to 0-100
  int left_pwm_value = map(left_wheel_speed, 0, 100, 0, 255);
  analogWrite(motor1_pwm_pin, left_pwm_value);

  right_wheel_speed = abs(right_wheel_speed); // Ensure positive value
  right_wheel_speed = constrain(right_wheel_speed, 0, 100); // Constrain to 0-100
  int right_pwm_value = map(right_wheel_speed, 0, 100, 0, 255);
  analogWrite(motor2_pwm_pin, right_pwm_value);
}

void loop() {
  // Example usage
  motorsSpeed(30, -30);//Change the value for negative values to reverse direction
  delay(2000); // Delay for 2 seconds
  motorsSpeed(0, 0);
  delay(2000); // Delay for 2 seconds
}