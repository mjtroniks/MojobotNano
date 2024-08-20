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

// Gyroscope control variables
float current_heading = 0;
float initial_heading = 0;

// Accelerometer control variables
float x_accel, y_accel, z_accel;
float x_position = 0;
float y_position = 0;
unsigned long previous_time;

// Control gains
float Kp_heading = 0.08;   // Proportional gain for heading correction
float Kp_position = 0.03;  // Proportional gain for position correction

// Correction factor to balance motor speeds
float motor_correction_factor = 0.975;  // Fine-tuned value to further reduce drift

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

    // Apply the correction factor to balance motor speeds
    left_wheel_speed = abs(left_wheel_speed);
    left_wheel_speed = constrain(left_wheel_speed, 0, 100);
    int left_pwm_value = map(left_wheel_speed, 0, 100, 0, 255);
    analogWrite(MOTOR1_PWM_PIN, left_pwm_value);

    right_wheel_speed = abs(right_wheel_speed);
    right_wheel_speed = constrain(right_wheel_speed, 0, 100);
    int right_pwm_value = map(right_wheel_speed, 0, 100, 0, 255 * motor_correction_factor);
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

// Function to correct the robot's direction and position
void correct_path() {
    // Calculate heading correction
    float heading_error = initial_heading - current_heading;
    int heading_correction = heading_error * Kp_heading;

    // Calculate position correction
    float position_error = x_position;  // Assuming y_position is the forward direction
    int position_correction = position_error * Kp_position;

    // Adjust motor speeds based on heading and position correction
    int left_motor_speed = 30 + heading_correction + position_correction;
    int right_motor_speed = 30 - heading_correction - position_correction;

    motors_speed(left_motor_speed, right_motor_speed);
}

// Function to rotate the robot 90 degrees to the right or left
void rotate_90_degrees(bool clockwise) {
    float x, y, z;
    IMU.readGyroscope(x, y, z);  // Read the initial gyroscope values

    float initial_angle = z * 180.0 / PI;  // Convert radians to degrees
    float current_angle = initial_angle;
    float target_angle = clockwise ? initial_angle + 90 : initial_angle - 90;

    Serial.print("Initial angle: ");
    Serial.println(initial_angle);
    Serial.print("Target angle: ");
    Serial.println(target_angle);

    // Begin rotating
    if (clockwise) {
        motors_speed(30, -30);  // Rotate right
        Serial.println("Rotating clockwise...");
    } else {
        motors_speed(-30, 30);  // Rotate left
        Serial.println("Rotating counterclockwise...");
    }

    while (abs(current_angle - initial_angle) < 90) {
        if (IMU.readGyroscope(x, y, z)) {
            current_angle += z * 0.013;  // Accumulate angle change
            Serial.print("Current angle: ");
            Serial.println(current_angle);
            delay(10);
        }
    }

    // Stop the motors
    motors_speed(0, 0);
    Serial.println("90 degrees reached");
    delay(2000); // Pause for 2 seconds before moving forward
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

    // Initialize timing for position updates
    previous_time = millis();
}

void loop() {
    long distance_cm = measure_distance();

    if (distance_cm >= 2 && distance_cm <= 3) {
        long distance_right, distance_left;

        // Rotate 90 degrees to the right
        rotate_90_degrees(true);
        distance_right = measure_distance();

        // Rotate 90 degrees back to the original position
        rotate_90_degrees(false);

        // Rotate 90 degrees to the left
        rotate_90_degrees(false);
        distance_left = measure_distance();

        // Rotate 90 degrees back to the original position
        rotate_90_degrees(true);

        // Compare distances and move towards the direction with greater distance
        if (distance_right > distance_left) {
            // Turn right
            rotate_90_degrees(true);
        } else {
            // Turn left
            rotate_90_degrees(false);
        }
    } else {
        // Move forward with straight path correction
        update_heading();
        update_position();
        correct_path();
    }

    // Add a small delay to avoid too frequent updates
    delay(10);
}
