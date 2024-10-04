import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;
import java.io.IOException;

public class LAB7_Line_Detection_LEDS extends PApplet {

    // Declare pins for left and right sensors
    private Pin leftSensorPin, rightSensorPin;

    // Declare pins for LEDs
    private Pin leftGreenPin, leftBluePin;
    private Pin rightGreenPin, rightBluePin;
    private IODevice arduino;

    public static void main(String[] args) {
        PApplet.main("LAB7_Line_Detection_LEDS");
    }

    public void settings() {
        size(800, 600);  // Set window size
    }

    public void setup() {
        String portName = "COM13"; // Adjust this as needed
        try {
            arduino = new FirmataDevice(portName);

            // Start communication with the Arduino
            arduino.start();
            arduino.ensureInitializationIsDone();

            println("Arduino initialized!");

            // Sensor pins
            leftSensorPin = arduino.getPin(5);  // D5 for left sensor
            rightSensorPin = arduino.getPin(4); // D4 for right sensor

            // Green and Blue LED pins
            leftGreenPin = arduino.getPin(10);  // D10 for Green on the left
            leftBluePin = arduino.getPin(17);   // A3 for Blue on the left
            rightGreenPin = arduino.getPin(21); // A7 for Green on the right
            rightBluePin = arduino.getPin(20);  // A6 for Blue on the right

            // Set LED pins to PWM mode
            leftGreenPin.setMode(Pin.Mode.PWM);
            leftBluePin.setMode(Pin.Mode.PWM);
            rightGreenPin.setMode(Pin.Mode.PWM);
            rightBluePin.setMode(Pin.Mode.PWM);

            // Set sensor pins to input mode
            leftSensorPin.setMode(Pin.Mode.INPUT);
            rightSensorPin.setMode(Pin.Mode.INPUT);

        } catch (IOException | InterruptedException e) {
            println("Error initializing Arduino");
            e.printStackTrace();
        }
    }

    public void draw() {
        background(255);  // White background

        // Control blue LEDs based on sensor readings
        controlBlueLEDs();

        // Draw sensor indicator squares
        drawSensorSquares();
    }

    // Function to control blue LEDs based on sensor readings
    public void controlBlueLEDs() {
        int trackingState = getTracking();

        try {
            if (trackingState == 0) {
                // Both sensors on white surface, blue LEDs are OFF
                leftBluePin.setValue(0);
                rightBluePin.setValue(0);
            } else {
                // Black line detected by either or both sensors, blue LEDs are ON
                if (trackingState == 1 || trackingState == 11) {
                    rightBluePin.setValue(255);  // Right sensor or both sensors triggered
                } else {
                    rightBluePin.setValue(0);
                }
                if (trackingState == 10 || trackingState == 11) {
                    leftBluePin.setValue(255);  // Left sensor or both sensors triggered
                } else {
                    leftBluePin.setValue(0);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Function to get tracking state based on sensor values
    public int getTracking() {
        int left = (int) leftSensorPin.getValue();
        int right = (int) rightSensorPin.getValue();

        if (left == 0 && right == 0) {
            return 0;  // Both sensors on white surface
        } else if (left == 0 && right == 1) {
            return 1;  // Right sensor triggered
        } else if (left == 1 && right == 0) {
            return 10; // Left sensor triggered
        } else if (left == 1 && right == 1) {
            return 11; // Both sensors triggered
        }
        return -1; // Fallback case
    }

    // Function to draw squares representing the sensor states
    public void drawSensorSquares() {
        float squareSize = 25;  // Define the size of the squares

        int trackingState = getTracking();

        // Left square color logic
        if (trackingState == 10 || trackingState == 11) {
            fill(0, 0, 255);  // Blue for left sensor triggered
        } else {
            fill(200);  // Light gray for inactive
        }
        rect(200 - squareSize, 400, squareSize, squareSize);  // Left sensor square

        // Right square color logic
        if (trackingState == 1 || trackingState == 11) {
            fill(0, 0, 255);  // Blue for right sensor triggered
        } else {
            fill(200);  // Light gray for inactive
        }
        rect(400 - squareSize, 400, squareSize, squareSize);  // Right sensor square
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
