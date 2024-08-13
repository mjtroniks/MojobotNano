#include <ArduinoBLE.h>

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

    // Set PWM frequency for motors (not available in ArduinoBLE)
}

void motors_speed(int left_wheel_speed, int right_wheel_speed) {
    // Control direction
    digitalWrite(motor1_dir_pin, left_wheel_speed > 0 ? HIGH : LOW);
    digitalWrite(motor2_dir_pin, right_wheel_speed > 0 ? HIGH : LOW);

    // Set PWM duty cycle
    int left_pwm_value = map(abs(left_wheel_speed), 0, 100, 0, 255);
    int right_pwm_value = map(abs(right_wheel_speed), 0, 100, 0, 255);
    analogWrite(motor1_pwm_pin, left_pwm_value);
    analogWrite(motor2_pwm_pin, right_pwm_value);
}

void loop() {
    // Example usage
    motors_speed(30, 30);
    delay(1000); // Delay for 1 second

    motors_speed(0, 0);
    delay(1000); // Delay for 1 second
    
    motors_speed(-30, 30);
    delay(1000); // Delay for 1 second


   motors_speed(0, 0);
    delay(1000); // Delay for 1 second

    motors_speed(30, -30);
    delay(1000); // Delay for 1 second

    motors_speed(0, 0);
    delay(1000); // Delay for 1 second
    
}
