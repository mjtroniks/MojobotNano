#include <Arduino.h>

// Ultrasonic sensor pins
const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;

// Line follower sensor pins
const int LEFT_SENSOR_PIN = 5;
const int RIGHT_SENSOR_PIN = 4;

// Motor 1 (Left Motor) pins
const int MOTOR1_PWM_PIN = 7;
const int MOTOR1_DIR_PIN = 8;

// Motor 2 (Right Motor) pins
const int MOTOR2_PWM_PIN = 6;
const int MOTOR2_DIR_PIN = 9;

// LEDs pins
const int LED_PIN_LEFT_RED = A2;
const int LED_PIN_LEFT_BLUE = A3;
const int LED_PIN_RIGHT_RED = 11;
const int LED_PIN_RIGHT_BLUE = A6;

// Set PWM frequency for motors (can be adjusted in setup if needed)
const int PWM_FREQUENCY = 1000;

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

void motors_speed(int left_wheel_speed, int right_wheel_speed) {
    // Control direction
    digitalWrite(MOTOR1_DIR_PIN, left_wheel_speed > 0 ? HIGH : LOW);
    digitalWrite(MOTOR2_DIR_PIN, right_wheel_speed > 0 ? HIGH : LOW);

    // Set PWM duty cycle
    left_wheel_speed = abs(left_wheel_speed);
    left_wheel_speed = constrain(left_wheel_speed, 0, 100);
    int left_pwm_value = map(left_wheel_speed, 0, 100, 0, 255);
    analogWrite(MOTOR1_PWM_PIN, left_pwm_value);

    right_wheel_speed = abs(right_wheel_speed);
    right_wheel_speed = constrain(right_wheel_speed, 0, 100);
    int right_pwm_value = map(right_wheel_speed, 0, 100, 0, 255);
    analogWrite(MOTOR2_PWM_PIN, right_pwm_value);
}

int get_tracking() {
    int left = digitalRead(LEFT_SENSOR_PIN);
    int right = digitalRead(RIGHT_SENSOR_PIN);

    if (left == LOW && right == LOW) {
        return 0;
    } else if (left == LOW && right == HIGH) {
        return 1;
    } else if (left == HIGH && right == LOW) {
        return 10;
    } else if (left == HIGH && right == HIGH) {
        return 11;
    }
    return -1; // Default return statement if none of the conditions are met
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);

    // Initialize ultrasonic sensor pins
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Initialize line follower sensor pins
    pinMode(LEFT_SENSOR_PIN, INPUT);
    pinMode(RIGHT_SENSOR_PIN, INPUT);

    // Initialize motor pins
    pinMode(MOTOR1_PWM_PIN, OUTPUT);
    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_PWM_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);

    // Initialize LED pins
    pinMode(LED_PIN_LEFT_RED, OUTPUT);
    pinMode(LED_PIN_LEFT_BLUE, OUTPUT);
    pinMode(LED_PIN_RIGHT_RED, OUTPUT);
    pinMode(LED_PIN_RIGHT_BLUE, OUTPUT);
}

void loop() {
    int tracking_state = get_tracking();
    long distance_cm = measure_distance();
    
    if (distance_cm > 3) {
        if (tracking_state == 0) {
            unsigned long timer = millis();  // Start timer when both sensors detect white

            while (millis() - timer < 500 && tracking_state == 0) {
                tracking_state = get_tracking();
                motors_speed(30, -30);  // Rotate in one direction
                digitalWrite(LED_PIN_LEFT_RED, LOW);
                digitalWrite(LED_PIN_RIGHT_RED, LOW);
                digitalWrite(LED_PIN_LEFT_BLUE, HIGH);
                digitalWrite(LED_PIN_RIGHT_BLUE, HIGH);
            }

            timer = millis();  // Start timer when both sensors detect white

            while (millis() - timer < 1000 && tracking_state == 0) {
                tracking_state = get_tracking();
                motors_speed(-30, 30);  // Switch direction
                digitalWrite(LED_PIN_LEFT_RED, LOW);
                digitalWrite(LED_PIN_RIGHT_RED, LOW);
                digitalWrite(LED_PIN_LEFT_BLUE, HIGH);
                digitalWrite(LED_PIN_RIGHT_BLUE, HIGH);
            }
        } else if (tracking_state == 1) {
            motors_speed(30, 10);  // Turn left
            digitalWrite(LED_PIN_LEFT_RED, HIGH);  // Turn on left LED
            digitalWrite(LED_PIN_RIGHT_RED, LOW);
        } else if (tracking_state == 10) {
            motors_speed(10, 30);  // Turn right
            digitalWrite(LED_PIN_LEFT_RED, LOW);
            digitalWrite(LED_PIN_RIGHT_RED, HIGH);  // Turn on right LED
        } else if (tracking_state == 11) {
            motors_speed(30, 30);  // Move forward
            digitalWrite(LED_PIN_LEFT_RED, HIGH);  // Turn on left LED
            digitalWrite(LED_PIN_RIGHT_RED, HIGH);  // Turn on right LED
        }
    } else if (distance_cm <= 3) {
        // Stop for 2 seconds, then move backward
        motors_speed(0, 0);  // Stop motors
        delay(2000);  // Wait for 2 seconds
        
        // Move backward for half a second
        motors_speed(-20, -20);  // Move backward
        delay(500);  // Move backward for half a second

        // Stop motors again briefly before continuing
        motors_speed(0, 0);
        delay(100);  // Short delay before resuming normal behavior
    }

    delay(100);  // Adjust delay as needed
}
