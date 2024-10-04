import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;

import java.io.IOException;

public class LAB4_PWM_LEDS extends PApplet {

    // Declare the pins as instance variables for Left and Right Green LEDs
    private Pin leftGreenPin, rightGreenPin;
    private IODevice arduino;
    private final int dutyStep = 5; // Step size for changing the duty cycle
    private int dutyCycle = 0;      // Current duty cycle value
    private boolean increasing = true; // To track if we're increasing or decreasing the duty cycle

    public static void main(String[] args) {
        PApplet.main("LAB4_PWM_LEDS");
    }

    public void settings() {
        size(600, 600);  // Set the size of the window
    }

    public void setup() {
        String portName = "COM13"; // Adjust this as needed
        try {
            arduino = new FirmataDevice(portName);

            // Start communication with the Arduino
            arduino.start();
            arduino.ensureInitializationIsDone();

            println("Arduino initialized!");

            // Initialize the Left and Right Green LED pins
            leftGreenPin = arduino.getPin(10);  // D10 for Green on the left
            rightGreenPin = arduino.getPin(21); // A7 for Green on the right

            // Set the LED pins to PWM mode
            leftGreenPin.setMode(Pin.Mode.PWM);
            rightGreenPin.setMode(Pin.Mode.PWM);

        } catch (IOException | InterruptedException e) {
            println("Error initializing Arduino");
            e.printStackTrace();
        }
    }

    public void draw() {
        background(255);  // White background


        // Update the PWM value and visual representation
        pwmGreenLED();

        // Add a delay to make the transition visible

    }

    public void pwmGreenLED() {
        try {
            // Update the duty cycle
            if (increasing) {
                dutyCycle += dutyStep;
                if (dutyCycle >= 255) {
                    dutyCycle = 255;
                    increasing = false;  // Start decreasing
                }
            } else {
                dutyCycle -= dutyStep;
                if (dutyCycle <= 0) {
                    dutyCycle = 0;
                    increasing = true;  // Start increasing
                }
            }

            // Apply the PWM value to both LEDs
            leftGreenPin.setValue(dutyCycle);
            rightGreenPin.setValue(dutyCycle);
            println("Green Duty Cycle: " + dutyCycle);

            // Draw the circles with the current green intensity

            drawGreenCircles(dutyCycle);
            delay(200);  // Delay 50 milliseconds between updates
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Draw circles representing the Green LEDs with color reflecting the PWM duty cycle
    public void drawGreenCircles(int dutyCycle) {
        // The duty cycle is a value between 0 and 255, representing brightness
        // Convert this to a green intensity and use it in the fill function
        float greenIntensity = map(dutyCycle, 0, 255, 0, 255);  // Map duty cycle to color intensity

        fill(0, greenIntensity, 0);  // Green color intensity
        ellipse(250, 300, 30, 30);  // Left green circle (size 30)
        ellipse(350, 300, 30, 30);  // Right green circle (size 30, closer by 2 cm)
    }

    public void stop() {
        try {
            arduino.stop();
        } catch (IOException e) {
            println("Error stopping Arduino");
            e.printStackTrace();
        }
        super.stop();
    }
}
