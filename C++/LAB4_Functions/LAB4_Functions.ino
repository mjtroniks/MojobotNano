// Function declarations
void setup();
void loop();
void blinkLed(int pin, int delayTime);

void setup() {
    // Initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    blinkLed(LED_BUILTIN, 1000); // Blink LED with 1 second delay
}

// Function definition
void blinkLed(int pin, int delayTime) {
    digitalWrite(pin, HIGH); // Turn the LED on (HIGH is the voltage level)
    delay(delayTime);        // Wait for a second
    digitalWrite(pin, LOW);  // Turn the LED off by making the voltage LOW
    delay(delayTime);        // Wait for a second
}
