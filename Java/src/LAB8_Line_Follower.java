import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;
import java.io.IOException;

public class LAB8_Line_Follower extends PApplet {

    // Declare pins for left and right sensors
    private Pin leftSensorPin, rightSensorPin;

    // Declare pins for left and right motors
    private Pin motor1_pwm_pin, motor1_dir_pin;
    private Pin motor2_pwm_pin, motor2_dir_pin;

    private Pin leftGreenPin, rightGreenPin;
    private IODevice arduino;
    private int leftSpeed = 15;  // Example motor speed for left wheel (0-100)
    private int rightSpeed = 0;  // Example motor speed for right wheel (0-100)
    private float lineYLeft = 150;    // For moving lines inside rectangles
    private float lineYRight = 150;   // For moving lines inside rectangles

    private final float CM_TO_PIXELS = 37.8f;  // 1 cm ≈ 37.8 pixels
    private final float RECT_WIDTH_CM = 0.5f;  // Width of the rectangle in cm
    private final float RECT_HEIGHT_CM = 2.0f;  // Height of the rectangle in cm
    private final float RECT_WIDTH_PIXELS = RECT_WIDTH_CM * CM_TO_PIXELS;  // Convert width to pixels
    private final float RECT_HEIGHT_PIXELS = RECT_HEIGHT_CM * CM_TO_PIXELS;  // Convert height to pixels

    public static void main(String[] args) {
        PApplet.main("LAB8_Line_Follower");
    }

    public void settings() {
        size(800, 600);  // Set window size
    }

    public void setup() {
        String portName = "COM13"; // Adjust this as needed
        try {
            arduino = new FirmataDevice(portName);
            arduino.start();
            arduino.ensureInitializationIsDone();

            println("Arduino initialized!");

            // Initialize motor and sensor pins
            motor1_pwm_pin = arduino.getPin(7);
            motor1_dir_pin = arduino.getPin(8);
            motor2_pwm_pin = arduino.getPin(6);
            motor2_dir_pin = arduino.getPin(9);

            leftSensorPin = arduino.getPin(5);
            rightSensorPin = arduino.getPin(4);

            leftGreenPin = arduino.getPin(10);
            rightGreenPin = arduino.getPin(21);

            motor1_pwm_pin.setMode(Pin.Mode.PWM);
            motor1_dir_pin.setMode(Pin.Mode.OUTPUT);
            motor2_pwm_pin.setMode(Pin.Mode.PWM);
            motor2_dir_pin.setMode(Pin.Mode.OUTPUT);

            leftGreenPin.setMode(Pin.Mode.PWM);
            rightGreenPin.setMode(Pin.Mode.PWM);

            leftSensorPin.setMode(Pin.Mode.INPUT);
            rightSensorPin.setMode(Pin.Mode.INPUT);

        } catch (IOException | InterruptedException e) {
            println("Error initializing Arduino");
            e.printStackTrace();
        }
    }

    public void draw() {
        background(255);  // White background

        // Draw sensor indicator squares
        int trackingState = drawSensorSquares();  // Call the function to draw squares and get the tracking state

        // Draw motor speed indicators (rectangles) further down
        drawMotorRectangles(trackingState);  // Pass the tracking state to update motor rectangles

        // Update the vertical line movement within the rectangles
        updateMotorRectangles();  // Update the moving lines
    }

    // Function to control motor speeds and directions
    public void motorsSpeed(int leftSpeed, int rightSpeed) {
        try {
            // Set direction for Motor 1 (left motor)
            if (leftSpeed > 0) {
                motor1_dir_pin.setValue(1);  // Forward
            } else {
                motor1_dir_pin.setValue(0);  // Backward
            }

            // Set direction for Motor 2 (right motor)
            if (rightSpeed > 0) {
                motor2_dir_pin.setValue(1);  // Forward
            } else {
                motor2_dir_pin.setValue(0);  // Backward
            }

            // Set PWM values based on speed (0-100 mapped to 0-255)
            int left_pwm_value = (int) map(abs(leftSpeed), 0, 100, 0, 255);  // Cast to int
            int right_pwm_value = (int) map(abs(rightSpeed), 0, 100, 0, 255);  // Cast to int

            motor1_pwm_pin.setValue(left_pwm_value);
            motor2_pwm_pin.setValue(right_pwm_value);

            // Adjust LED intensity based on motor speed (optional visual feedback)
            leftGreenPin.setValue(left_pwm_value);
            rightGreenPin.setValue(right_pwm_value);

            println("Left Motor Speed: " + leftSpeed + " | Right Motor Speed: " + rightSpeed);

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

    // Function to draw squares representing the sensor states, centered on the screen and 0.5 cm inward, moved 2 cm down
    public int drawSensorSquares() {
        float squareSize = 25;  // Define the size of the squares

        // Center squares horizontally and vertically, 0.5 cm inward, and 2 cm down
        float leftSquareX = width / 2 - 75 - squareSize / 2 + CM_TO_PIXELS * 0.5f;  // Move 0.5 cm inward
        float rightSquareX = width / 2 + 75 - squareSize / 2 - CM_TO_PIXELS * 0.5f;  // Move 0.5 cm inward
        float squareY = height / 2 - 50 + CM_TO_PIXELS * 4.0f;  // Adjust squares vertically, 2 cm down

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

        return trackingState;
    }

    // Draw the rectangles representing the motors, further down from the squares, and update motor speeds
    public void drawMotorRectangles(int trackingState) {
        fill(200);  // Light gray for the rectangles

        // Position the motor rectangles further down from the sensor squares
        float leftX = width / 2 - 100 - RECT_WIDTH_PIXELS / 2;  // Centered 100 pixels left of center
        float rightX = width / 2 + 100 - RECT_WIDTH_PIXELS / 2;  // Centered 100 pixels right of center
        float rectY = height / 2 + 100 + CM_TO_PIXELS * 2.0f;  // Move motor rectangles further down

        rect(leftX, rectY, RECT_WIDTH_PIXELS, RECT_HEIGHT_PIXELS);  // Left motor rectangle
        rect(rightX, rectY, RECT_WIDTH_PIXELS, RECT_HEIGHT_PIXELS);  // Right motor rectangle

        // Update motor speeds based on tracking state
        switch (trackingState) {
            case 0:  // No line detected
                leftSpeed = 0;
                rightSpeed = 0;
                break;
            case 1:  // Right sensor detected (turn left)
                leftSpeed = 20;
                rightSpeed = 10;
                break;
            case 10:  // Left sensor detected (turn right)
                leftSpeed = 10;
                rightSpeed = 20;
                break;
            case 11:  // Both sensors detected (go straight)
                leftSpeed = 20;
                rightSpeed = 20;
                break;
            default:
                leftSpeed = 0;
                rightSpeed = 0;
                break;
        }

        // Print the current motor speeds for debugging
        println("Tracking State: " + trackingState + " | LeftSpeed: " + leftSpeed + " | RightSpeed: " + rightSpeed);

        // Set the motor speeds using the motorsSpeed function
        motorsSpeed(leftSpeed, rightSpeed);

        // Add moving lines inside the rectangles based on motor speed
        stroke(0);
        line(leftX, lineYLeft + rectY - 150, leftX + RECT_WIDTH_PIXELS, lineYLeft + rectY - 150);  // Left motor vertical line
        line(rightX, lineYRight + rectY - 150, rightX + RECT_WIDTH_PIXELS, lineYRight + rectY - 150);  // Right motor vertical line
    }


    // Update the moving lines within the rectangles based on motor speed
    // Update the moving lines within the rectangles based on motor speed
    public void updateMotorRectangles() {
        // Calculate speed based on motor PWM values
        int leftMotorSpeed = (int) map(abs(leftSpeed), 0, 100, 1, 10);  // Scale to control line speed
        int rightMotorSpeed = (int) map(abs(rightSpeed), 0, 100, 1, 10);  // Ensure right speed is mapped properly

        // Debugging: Print the speeds and line values for verification
        println("Left Speed: " + leftSpeed + " | Right Speed: " + rightSpeed);
        println("LineYLeft: " + lineYLeft + " | LineYRight: " + lineYRight);

        // Move the lines for the left motor
        if (leftSpeed > 0) {
            lineYLeft -= leftMotorSpeed;  // Move up when speed is positive
        } else if (leftSpeed < 0) {
            lineYLeft += leftMotorSpeed;  // Move down when speed is negative
        }

        // Move the lines for the right motor
        if (rightSpeed > 0) {
            lineYRight -= rightMotorSpeed;  // Move up when speed is positive
        } else if (rightSpeed < 0) {
            lineYRight += rightMotorSpeed;  // Move down when speed is negative
        }

        // Reset line positions when they exit the top or bottom of the rectangle for left motor
        if (lineYLeft < 150) lineYLeft = 150 + RECT_HEIGHT_PIXELS;  // Wrap from top to bottom
        if (lineYLeft > 150 + RECT_HEIGHT_PIXELS) lineYLeft = 150;  // Wrap from bottom to top

        // Reset line positions when they exit the top or bottom of the rectangle for right motor
        if (lineYRight < 150) lineYRight = 150 + RECT_HEIGHT_PIXELS;  // Wrap from top to bottom
        if (lineYRight > 150 + RECT_HEIGHT_PIXELS) lineYRight = 150;  // Wrap from bottom to top
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
