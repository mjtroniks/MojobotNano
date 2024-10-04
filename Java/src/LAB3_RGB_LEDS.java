/*
 * LAB3_RGB_LEDS: Arduino Nano 33 BLE Green and Blue LEDs Control
 *
 * Explanation:
 * The Arduino Nano 33 BLE has multiple pins capable of PWM, but the hardware limits the number of
 * simultaneous PWM channels to 4. This code will control only the green and blue LEDs on the left and right
 * side, for a total of 4 LEDs.
 * By setting these 4 LEDs (two left and two right) to PWM mode, we stay within the 4-channel hardware limit.
 * This ensures the program runs smoothly without exceeding the available PWM resources on the Arduino.
 * The visual representation is now 2 circles, one for the left LEDs and one for the right LEDs, changing colors accordingly.
 */

import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;

import java.io.IOException;

public class LAB3_RGB_LEDS extends PApplet {

    // Declare the pins as instance variables for Left and Right Green and Blue LEDs
    private Pin leftGreenPin, leftBluePin;
    private Pin rightGreenPin, rightBluePin;
    private IODevice arduino;
    private long lastSwitchTime = 0;
    private int currentColor = 0; // 0 = Green, 1 = Blue
    private int switchInterval = 2000; // Time interval for color switching (in milliseconds)

    public static void main(String[] args) {
        PApplet.main("LAB3_RGB_LEDS");
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

            // Initialize the Left RGB LED pins (Green and Blue)
            leftGreenPin = arduino.getPin(10); // D10 for Green
            leftBluePin = arduino.getPin(17); // A3 for Blue

            // Initialize the Right RGB LED pins (Green and Blue)
            rightGreenPin = arduino.getPin(21); // A7 for Green
            rightBluePin = arduino.getPin(20); // A6 for Blue

            // Set the RGB pins to PWM mode
            leftGreenPin.setMode(Pin.Mode.PWM);
            leftBluePin.setMode(Pin.Mode.PWM);
            rightGreenPin.setMode(Pin.Mode.PWM);
            rightBluePin.setMode(Pin.Mode.PWM);

        } catch (IOException | InterruptedException e) {
            println("Error initializing Arduino");
            e.printStackTrace();
        }
    }

    public void draw() {
        background(255);  // White background

        // Only change LEDs and colors after the switch interval has passed
        if (millis() - lastSwitchTime >= switchInterval) {
            lastSwitchTime = millis();  // Reset the timer

            // Cycle between green and blue
            switch (currentColor) {
                case 0:
                    setGreen();   // Turn on green LEDs
                    currentColor = 1;
                    break;
                case 1:
                    setBlue();    // Turn on blue LEDs
                    currentColor = 0;
                    break;
            }
        }

        // Draw the color based on the currentColor state
        switch (currentColor) {
            case 0:
                delay(2000);  // Delay for 2 seconds to keep the green circle on screen
                drawGreenCircles();  // Draw circles with green color
                break;
            case 1:
                delay(2000);  // Delay for 2 seconds to keep the blue circle on screen
                drawBlueCircles();   // Draw circles with blue color
                break;
        }
    }

    // Set both Left and Right LEDs to Green
    public void setGreen() {
        try {
            // Turn on Green LEDs, turn off Blue LEDs
            leftGreenPin.setValue(255);
            leftBluePin.setValue(0);

            rightGreenPin.setValue(255);
            rightBluePin.setValue(0);

            println("Green LEDs ON");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Set both Left and Right LEDs to Blue
    public void setBlue() {
        try {
            // Turn on Blue LEDs, turn off Green LEDs
            leftGreenPin.setValue(0);
            leftBluePin.setValue(255);

            rightGreenPin.setValue(0);
            rightBluePin.setValue(255);

            println("Blue LEDs ON");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Draw circles representing the Green LEDs
    public void drawGreenCircles() {
        fill(0, 255, 0);  // Green color
        ellipse(250, 300, 30, 30);  // Green circle for Left LED (size 30)
        ellipse(350, 300, 30, 30);  // Green circle for Right LED (size 30, closer by 2 cm)
    }

    // Draw circles representing the Blue LEDs
    public void drawBlueCircles() {
        fill(0, 0, 255);  // Blue color
        ellipse(250, 300, 30, 30);  // Blue circle for Left LED (size 30)
        ellipse(350, 300, 30, 30);  // Blue circle for Right LED (size 30, closer by 2 cm)
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
