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

// Gyroscope control variables
float initial_heading = 0;
float current_heading = 0;

// Target speed
const int TARGET_SPEED = 50;  // Define your desired speed here (0-100)

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

// Function to read the gyroscope and calculate the current heading
void update_heading() {
    float x, y, z;
    if (IMU.readGyroscope(x, y, z)) {
        current_heading += z * 0.013;  // Adjust this factor based on your needs
    }
}

// Function to correct the robot's heading without changing the overall speed
void correct_heading() {
    float heading_error = initial_heading - current_heading;
    const float correction_threshold = 2.0;  // Threshold in degrees to apply correction

    if (heading_error > correction_threshold) {
        // Robot is veering left, briefly slow down the left wheel to correct
        motors_speed(TARGET_SPEED - 10, TARGET_SPEED);  // Decrease left wheel speed temporarily
        delay(50);  // Apply correction for a short duration
    } else if (heading_error < -correction_threshold) {
        // Robot is veering right, briefly slow down the right wheel to correct
        motors_speed(TARGET_SPEED, TARGET_SPEED - 10);  // Decrease right wheel speed temporarily
        delay(50);  // Apply correction for a short duration
    }

    // Restore both motors to the same speed after correction
    motors_speed(TARGET_SPEED, TARGET_SPEED);
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

    // Set initial heading
    float x, y, z;
    if (IMU.readGyroscope(x, y, z)) {
        initial_heading = z * 180.0 / PI;  // Convert radians to degrees
    }

    // Set the motors to the target speed
    motors_speed(TARGET_SPEED, TARGET_SPEED);
}

void loop() {
    // Update the current heading
    update_heading();

    // Correct the robot's heading to maintain a straight line
    correct_heading();

    // Optionally, measure distance to avoid obstacles
    long distance_cm = measure_distance();
    if (distance_cm < 10) {
        motors_speed(0, 0);  // Stop if an obstacle is detected
    }

    // Add a small delay to avoid too frequent updates
    delay(10);
}
