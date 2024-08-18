#include <Arduino.h>

// Function to initialize PWM pins for RGB LEDs
void init_pwm(uint8_t pin) {
    pinMode(pin, OUTPUT);
    
}

// Set up PWM for left and right RGB LEDs
const int pwm_red_left = A2;
const int pwm_green_left = D10;
const int pwm_blue_left = A3;

const int pwm_red_right = D11;
const int pwm_green_right = A7;
const int pwm_blue_right = A6;

void setup() {
    init_pwm(pwm_red_left);
    init_pwm(pwm_green_left);
    //init_pwm(pwm_blue_left);
    init_pwm(pwm_red_right);
    init_pwm(pwm_green_right);
    //init_pwm(pwm_blue_right);
}

// Function to set RGB values for LEDs
void set_rgb(int pwm_red, int pwm_green, int pwm_blue, int red, int green, int blue) {
    analogWrite(pwm_red, red);    // Set duty cycle for red LED
    analogWrite(pwm_green, green);  // Set duty cycle for green LED
    //analogWrite(pwm_blue, blue);   // Set duty cycle for blue LED
}

// Function to map input value to PWM range
int map_input(int value) {
    return map(constrain(value, 0, 255), 0, 255, 0, 255);
}

void loop() {
    // Set RGB values for both LEDs
    int red_value = map_input(255);    // Maximum intensity for red
    int green_value = map_input(255);    // No intensity for green
    int blue_value = map_input(0);   // Maximum intensity for blue

    // Set RGB values for left and right LEDs
    set_rgb(pwm_red_left, pwm_green_left, pwm_blue_left, red_value, green_value, blue_value);
    set_rgb(pwm_red_right, pwm_green_right, pwm_blue_right, red_value, green_value, blue_value);

    delay(2000);  // Wait for 2 seconds
}