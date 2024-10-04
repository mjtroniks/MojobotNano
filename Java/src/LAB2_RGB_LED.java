import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;

import java.io.IOException;

public class LAB2_RGB_LED extends PApplet {

    // Declare the pins as instance variables
    private Pin RPin;
    private Pin GPin;
    private Pin BPin;
    private IODevice arduino;
    private long lastSwitchTime = 0;
    private int currentColor = 0; // 0 = Red, 1 = Green, 2 = Blue
    private int switchInterval = 1000; // Time interval for color switching (in milliseconds)

    public static void main(String[] args) {
        PApplet.main("LAB2_RGB_LED");
    }

    public void settings() {
        size(600, 600);  // Set the size of the window
    }

    public void setup() {
        String portName = "COM13";
        try {
            arduino = new FirmataDevice(portName);

            // Start communication with the Arduino
            arduino.start();
            arduino.ensureInitializationIsDone();

            println("Arduino initialized!");

            // Initialize the RGB pins
            RPin = arduino.getPin(11); // A2 for Red
            GPin = arduino.getPin(21); // Pin 10 for Green
            BPin = arduino.getPin(20); // A3 for Blue

            // Set the RGB pins to PWM mode
            RPin.setMode(Pin.Mode.PWM);
            GPin.setMode(Pin.Mode.PWM);
            BPin.setMode(Pin.Mode.PWM);
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

            // Cycle through red, green, and blue
            switch (currentColor) {
                case 0:
                    redLedBlink();   // Turn on red LED and draw red rectangle
                    currentColor = 1;
                    break;
                case 1:
                    greenBlink();    // Turn on green LED and draw green rectangle
                    currentColor = 2;
                    break;
                case 2:
                    blueBlink();     // Turn on blue LED and draw blue rectangle
                    currentColor = 0;
                    break;
            }
        }
    }

    public void redLedBlink() {
        try {
            // Turn on Red LED, turn off others
            delay(1000);  // Delay for 1 second to keep the red square on screen
            RPin.setValue(255);
            GPin.setValue(0);
            BPin.setValue(0);
            RRectangle();
            println("Red LED ON");


        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void greenBlink() {
        try {
            // Turn on Green LED, turn off others
            delay(1000);  // Delay for 1 second to keep the green square on screen
            RPin.setValue(0);
            GPin.setValue(255);
            BPin.setValue(0);
            GRectangle();
            println("Green LED ON");


        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void blueBlink() {
        try {
            // Turn on Blue LED, turn off others
            delay(1000);  // Delay for 1 second to keep the blue square on screen
            RPin.setValue(0);
            GPin.setValue(0);
            BPin.setValue(255);
            BRectangle();
            println("Blue LED ON");


        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void RRectangle() {
        fill(255, 0, 0);  // Red color
        rect(50, 150, 100, 100);  // Red square
        fill(255);  // White color for other squares
        rect(150, 150, 100, 100);  // Green square (off)
        rect(250, 150, 100, 100);  // Blue square (off)
    }

    public void GRectangle() {
        fill(255);  // White color for other squares
        rect(50, 150, 100, 100);  // Red square (off)
        fill(0, 255, 0);  // Green color
        rect(150, 150, 100, 100);  // Green square
        fill(255);  // White color for other squares
        rect(250, 150, 100, 100);  // Blue square (off)
    }

    public void BRectangle() {
        fill(255);  // White color for other squares
        rect(50, 150, 100, 100);  // Red square (off)
        rect(150, 150, 100, 100);  // Green square (off)
        fill(0, 0, 255);  // Blue color
        rect(250, 150, 100, 100);  // Blue square

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
