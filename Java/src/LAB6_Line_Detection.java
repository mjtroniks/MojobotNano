import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;
import java.io.IOException;

public class LAB6_Line_Detection extends PApplet {

    // Declare pins for left and right sensors
    private Pin leftSensorPin, rightSensorPin;



    private Pin leftGreenPin, rightGreenPin;
    private IODevice arduino;

    public static void main(String[] args) {
        PApplet.main("LAB6_Line_Detection");
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

        // Draw motor speed indicators (rectangles)


        // Draw sensor indicator squares
        drawSensorSquares();


        // Update the vertical line movement within the rectangles

    }

    // Function to get tracking state based on sensor values
    public int getTracking() {
        int left = (int) leftSensorPin.getValue();
        int right = (int) rightSensorPin.getValue();

        if (left == 0 && right == 0) {
            return 0;
        } else if (left == 0 && right == 1) {
            return 1;
        } else if (left == 1 && right == 0) {
            return 10;
        } else if (left == 1 && right == 1) {
            return 11;
        }
        return -1; // Fallback case
    }

    // Function to control motor speeds and directions


    // Function to draw squares representing the sensor states
    public void drawSensorSquares() {
        float squareSize = 25;  // Define the size of the squares
        float squareOffset = 50;  // Distance to position squares near the motor rectangles

        int trackingState = getTracking();

        // Left square color logic
        if (trackingState == 10 || trackingState == 11) {
            fill(255, 0, 0);  // Red for left sensor triggered
        } else {
            fill(200);  // Light gray for inactive
        }
        rect(200 - squareSize, 400, squareSize, squareSize);  // Left sensor square

        // Right square color logic
        if (trackingState == 1 || trackingState == 11) {
            fill(255, 0, 0);  // Red for right sensor triggered
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