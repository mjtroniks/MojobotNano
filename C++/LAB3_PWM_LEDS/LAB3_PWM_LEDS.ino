
const int ledGL = 10;  // Green LED for RGB Left (PWM-capable pin)
const int ledGR = A7; // Green LED for RGB Right (PWM-capable pin)
const int dutyStep = 5; // Step size for changing the duty cycle

void setup() {
  pinMode(ledGL, OUTPUT);
  pinMode(ledGR, OUTPUT);
  
  Serial.begin(9600); // Initialize serial communication for debugging
  Serial.println("Setup complete.");
}

void loop() {
  pwmGreen();
  delay(1000); // Delay between each PWM cycle, adjust as necessary
}

void pwmGreen() {
  Serial.println("Starting Green LED PWM.");

  // Increase the duty cycle gradually for Green LEDs
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle += dutyStep) {
    analogWrite(ledGL, dutyCycle);
    analogWrite(ledGR, dutyCycle);
    
    Serial.print("Green Duty Cycle (Increase): ");
    Serial.println(dutyCycle);
    
    delay(10); // Adjust delay time if necessary
  }

  Serial.println("Green PWM Increase completed.");

  // Decrease the duty cycle gradually for Green LEDs
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle -= dutyStep) {
    analogWrite(ledGL, dutyCycle);
    analogWrite(ledGR, dutyCycle);
    
    Serial.print("Green Duty Cycle (Decrease): ");
    Serial.println(dutyCycle);
    
    delay(10); // Adjust delay time if necessary
  }

  Serial.println("Green PWM Decrease completed.");
  Serial.println("Green LED PWM completed.");
}
