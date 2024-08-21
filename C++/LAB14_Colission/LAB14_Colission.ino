#include <Arduino.h>
#include <Arduino_LSM9DS1.h>  // Include the library for the built-in accelerometer and gyroscope

// Motor pins
const int MOTOR1_PWM_PIN = 7;
const int MOTOR1_DIR_PIN = 8;
const int MOTOR2_PWM_PIN = 6;
const int MOTOR2_DIR_PIN = 9;

// Gyroscope and Accelerometer control variables
float current_heading = 0;
float initial_heading = 0;
float x_accel = 0, y_accel = 0, z_accel = 0;  // Initialize accelerometer variables
float filtered_x_accel = 0;
float alpha = 0.1;  // Smoothing factor for low-pass filter
float x_position = 0;
float y_position = 0;
unsigned long previous_time = 0;

// Collision detection threshold
const float collision_threshold = 0.15;  // Adjusted for sensitivity

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

// Function to update heading based on gyroscope data
void update_heading() {
    float x, y, z;
    if (IMU.readGyroscope(x, y, z)) {
        current_heading += z * 0.013;  // Adjust this factor based on your needs
    }
}

// Function to update position based on accelerometer data
void update_position() {
    float dt = (millis() - previous_time) / 1000.0;
    previous_time = millis();
    
    if (IMU.readAcceleration(x_accel, y_accel, z_accel)) {
        x_position += x_accel * dt * dt;
        y_position += y_accel * dt * dt;
    }
}

// Function to filter accelerometer data for collision detection
void filter_acceleration() {
    filtered_x_accel = alpha * filtered_x_accel + (1 - alpha) * x_accel;
}

// Function to detect collision and determine the direction
void detect_collision() {
    if (IMU.readAcceleration(x_accel, y_accel, z_accel)) {
        filter_acceleration();

        if (abs(filtered_x_accel) > collision_threshold) {
            if (filtered_x_accel > 0) {
                // Left-side collision detected
                Serial.println("Left-side collision detected!");
                reverse_and_turn(15, true);  // Turn right by 15 degrees
            } else if (filtered_x_accel < 0) {
                // Right-side collision detected
                Serial.println("Right-side collision detected!");
                reverse_and_turn(15, false);  // Turn left by 15 degrees
            }
        }
    }
}

// Function to reverse and turn based on collision
void reverse_and_turn(int angle, bool turn_right) {
    // Stop the motors
    motors_speed(0, 0);
    delay(500);

    // Reverse slightly
    motors_speed(-30, -30);
    delay(500);

    // Turn 15 degrees based on collision direction
    if (turn_right) {
        motors_speed(30, -30);  // Turn right
    } else {
        motors_speed(-30, 30);  // Turn left
    }
    delay(300);  // Adjust based on motor speed and desired turn angle

    // Resume forward movement
    motors_speed(30, 30);
}

void setup() {
    Serial.begin(115200);

    pinMode(MOTOR1_PWM_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_PWM_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);

    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }

    previous_time = millis();
    motors_speed(30, 30);
}

void loop() {
    update_heading();
    update_position();

    // Detect and handle collisions
    detect_collision();

    // Add a small delay to avoid too frequent updates
    delay(10);
}
