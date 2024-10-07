import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;
import java.io.IOException;

public class LAB7_Line_Detection_LEDS extends PApplet {

    // Declare pins for left and right sensors
    private Pin leftSensorPin, rightSensorPin;

    // Declare pins for LEDs
    private Pin leftGreenPin, rightGreenPin;
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

            // Green LED pins
            leftGreenPin = arduino.getPin(10);  // D10 for Green on the left
            rightGreenPin = arduino.getPin(21); // A7 for Green on the right

            // Set LED pins to PWM mode
            leftGreenPin.setMode(Pin.Mode.PWM);
            rightGreenPin.setMode(Pin.Mode.PWM);

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

        // Control green LEDs based on sensor readings
        controlGreenLEDs();

        // Draw sensor indicator squares
        drawSensorSquares();
    }

    // Function to control green LEDs based on sensor readings
    public void controlGreenLEDs() {
        int trackingState = getTracking();

        try {
            if (trackingState == 0) {
                // Both sensors on white surface, green LEDs are OFF
                leftGreenPin.setValue(0);
                rightGreenPin.setValue(0);
            } else {
                // Black line detected by either or both sensors, green LEDs are ON
                if (trackingState == 1 || trackingState == 11) {
                    rightGreenPin.setValue(255);  // Right sensor or both sensors triggered
                } else {
                    rightGreenPin.setValue(0);
                }
                if (trackingState == 10 || trackingState == 11) {
                    leftGreenPin.setValue(255);  // Left sensor or both sensors triggered
                } else {
                    leftGreenPin.setValue(0);
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

    // Function to draw squares representing the sensor states, centered on the screen
    public void drawSensorSquares() {
        float squareSize = 25;  // Set the size of the squares back to 25

        // Calculate positions to center the squares
        float leftSquareX = width / 2 - 75;  // 75 pixels left from center for the left square
        float rightSquareX = width / 2 + 50;  // 50 pixels right from center for the right square
        float squareY = height / 2 - squareSize / 2;  // Vertically center the squares

        int trackingState = getTracking();

        // Left square color logic
        if (trackingState == 10 || trackingState == 11) {
            fill(0, 255, 0);  // Green for left sensor triggered
        } else {
            fill(200);  // Light gray for inactive
        }
        rect(leftSquareX, squareY, squareSize, squareSize);  // Left sensor square

        // Right square color logic
        if (trackingState == 1 || trackingState == 11) {
            fill(0, 255, 0);  // Green for right sensor triggered
        } else {
            fill(200);  // Light gray for inactive
        }
        rect(rightSquareX, squareY, squareSize, squareSize);  // Right sensor square
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
