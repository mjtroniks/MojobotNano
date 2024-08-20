//Mjtroniks
#include <Arduino.h>
#include <Arduino_LSM9DS1.h>  // Include the library for the built-in accelerometer and gyroscope

// Ultrasonic sensor pins
const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;

// Motor pins
const int MOTOR1_PWM_PIN = 7;
const int MOTOR1_DIR_PIN = 8;
const int MOTOR2_PWM_PIN = 6;
const int MOTOR2_DIR_PIN = 9;

// LEDs pins
const int LED_PIN_LEFT = A2;
const int LED_PIN_RIGHT = D11;
const int LED_PIN_LEFT_BLUE = A3;
const int LED_PIN_RIGHT_BLUE = A6;

// Set PWM frequency for motors
const int PWM_FREQUENCY = 1000;

// Function to measure distance using ultrasonic sensor
long measure_distance() {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance_cm = duration * 0.034 / 2;
    return distance_cm;
}

// Function to set motor speeds
void motors_speed(int left_wheel_speed, int right_wheel_speed) {
    if (left_wheel_speed > 0) {
        digitalWrite(MOTOR1_DIR_PIN, HIGH);
    } else {
        digitalWrite(MOTOR1_DIR_PIN, LOW);
    }

    if (right_wheel_speed > 0) {
        digitalWrite(MOTOR2_DIR_PIN, HIGH);
    } else {
        digitalWrite(MOTOR2_DIR_PIN, LOW);
    }

    left_wheel_speed = abs(left_wheel_speed);
    left_wheel_speed = constrain(left_wheel_speed, 0, 100);
    int left_pwm_value = map(left_wheel_speed, 0, 100, 0, 255);
    analogWrite(MOTOR1_PWM_PIN, left_pwm_value);

    right_wheel_speed = abs(right_wheel_speed);
    right_wheel_speed = constrain(right_wheel_speed, 0, 100);
    int right_pwm_value = map(right_wheel_speed, 0, 100, 0, 255);
    analogWrite(MOTOR2_PWM_PIN, right_pwm_value);
}

// Function to rotate the robot 90 degrees to the right or left
void rotate_90_degrees(bool clockwise) {
    float x, y, z;
    IMU.readGyroscope(x, y, z);  // Read the initial gyroscope values

    float initial_angle = z;  // Gyroscope Z-axis is typically used for rotation around the vertical axis
    float current_angle = initial_angle;
    float target_angle = clockwise ? initial_angle + 90 : initial_angle - 90;

    // Begin rotating
    if (clockwise) {
        motors_speed(30, -30);  // Rotate right
    } else {
        motors_speed(-30, 30);  // Rotate left
    }

    while (abs(current_angle - target_angle) > 1) {
        if (IMU.readGyroscope(x, y, z)) {
            current_angle = z;
        }
        delay(10);
    }

    // Stop the motors
    motors_speed(0, 0);
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);

    // Initialize ultrasonic sensor pins
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Initialize motor pins
    pinMode(MOTOR1_PWM_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_PWM_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);

    // Initialize LED pins
    pinMode(LED_PIN_LEFT, OUTPUT);
    pinMode(LED_PIN_RIGHT, OUTPUT);
    pinMode(LED_PIN_LEFT_BLUE, OUTPUT);
    pinMode(LED_PIN_RIGHT_BLUE, OUTPUT);

    // Initialize IMU (accelerometer and gyroscope)
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
}

void loop() {
    long distance_cm = measure_distance();
    //Serial.println(distance_cm);
    if (distance_cm >= 2 && distance_cm <= 3) {
        long distance_right, distance_left;

        // Rotate 90 degrees to the right
        rotate_90_degrees(true);
        delay(500);
        distance_right = measure_distance();

        // Rotate 90 degrees back to the original position
        rotate_90_degrees(false);
        delay(500);

        // Rotate 90 degrees to the left
        rotate_90_degrees(false);
        delay(500);
        distance_left = measure_distance();

        // Rotate 90 degrees back to the original position
        rotate_90_degrees(true);
        delay(500);

        // Compare distances and move towards the direction with greater distance
        if (distance_right > distance_left) {
            // Turn right
            rotate_90_degrees(true);
        } else {
            // Turn left
            rotate_90_degrees(false);
        }
    } else {
        // Move forward
        motors_speed(30, 30);
    }

    delay(100);  // Adjust delay as needed
}
//