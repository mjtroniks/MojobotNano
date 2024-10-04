import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;
import java.io.IOException;

public class LAB5_MotorControl extends PApplet {

    // Declare pins for left and right motors
    private Pin motor1_pwm_pin, motor1_dir_pin;
    private Pin motor2_pwm_pin, motor2_dir_pin;

    private Pin leftGreenPin, rightGreenPin;
    private IODevice arduino;
    private int leftSpeed = 15;  // Example motor speed for left wheel (0-100)
    private int rightSpeed = 0;  // Example motor speed for right wheel (0-100)
    private float lineYLeft = 150;    // For moving lines inside rectangles
    private float lineYRight = 150;   // For moving lines inside rectangles
    private long lastUpdate = 0;      // For non-blocking timing

    // Constants for conversions from cm to pixels
    private final float CM_TO_PIXELS = 37.8f;  // 1 cm ≈ 37.8 pixels
    private final float RECT_WIDTH_CM = 0.5f;  // Width of the rectangle in cm
    private final float RECT_HEIGHT_CM = 2.0f;  // Height of the rectangle in cm
    private final float RECT_WIDTH_PIXELS = RECT_WIDTH_CM * CM_TO_PIXELS;  // Convert width to pixels
    private final float RECT_HEIGHT_PIXELS = RECT_HEIGHT_CM * CM_TO_PIXELS;  // Convert height to pixels
    private final float RECT_DISTANCE_ADJUST_CM = 4.0f;  // Distance to move rectangles closer by 4 cm (2 cm adjustment)

    public static void main(String[] args) {
        PApplet.main("LAB5_MotorControl");
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

            // Motor pins
            motor1_pwm_pin = arduino.getPin(7);  // PWM for motor 1
            motor1_dir_pin = arduino.getPin(8);  // Direction for motor 1
            motor2_pwm_pin = arduino.getPin(6);  // PWM for motor 2
            motor2_dir_pin = arduino.getPin(9);  // Direction for motor 2

            // Green LED pins
            leftGreenPin = arduino.getPin(10);  // D10 for Green on the left
            rightGreenPin = arduino.getPin(21); // A7 for Green on the right

            // Set the motor and LED pins to PWM mode
            motor1_pwm_pin.setMode(Pin.Mode.PWM);
            motor1_dir_pin.setMode(Pin.Mode.OUTPUT);
            motor2_pwm_pin.setMode(Pin.Mode.PWM);
            motor2_dir_pin.setMode(Pin.Mode.OUTPUT);

            leftGreenPin.setMode(Pin.Mode.PWM);
            rightGreenPin.setMode(Pin.Mode.PWM);

        } catch (IOException | InterruptedException e) {
            println("Error initializing Arduino");
            e.printStackTrace();
        }
    }

    public void draw() {
        background(255);  // White background

        // Draw motor speed indicators (rectangles)
        drawMotorRectangles();

        // Control motor speeds and directions
        motorsSpeed(leftSpeed, rightSpeed);

        // Update the vertical line movement within the rectangles
        updateMotorRectangles();


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

    // Draw the rectangles representing the motors
    public void drawMotorRectangles() {
        fill(200);  // Light gray for the rectangles

        // Adjust position of the rectangles to be 0.5 cm outwards from the circles and 2 cm below
        float leftX = 250 - (RECT_WIDTH_PIXELS / 2) - (CM_TO_PIXELS * 0.5f);  // Left motor rectangle (0.5 cm left of the circle)
        float rightX = 350 - (RECT_WIDTH_PIXELS / 2) + (CM_TO_PIXELS * 0.5f);  // Right motor rectangle (0.5 cm right of the circle)
        float rectY = 300 + (CM_TO_PIXELS * 2.0f);  // Move 2 cm below the circles

        rect(leftX, rectY, RECT_WIDTH_PIXELS, RECT_HEIGHT_PIXELS);  // Left motor rectangle
        rect(rightX, rectY, RECT_WIDTH_PIXELS, RECT_HEIGHT_PIXELS);  // Right motor rectangle

        // Add moving lines inside the rectangles based on motor speed
        stroke(0);
        line(leftX, lineYLeft + rectY - 150, leftX + RECT_WIDTH_PIXELS, lineYLeft + rectY - 150);    // Left motor vertical line
        line(rightX, lineYRight + rectY - 150, rightX + RECT_WIDTH_PIXELS, lineYRight + rectY - 150);  // Right motor vertical line
    }


    // Update the moving lines within the rectangles based on motor speed
    public void updateMotorRectangles() {
        // Calculate speed based on motor PWM values
        int leftMotorSpeed = (int) map(abs(leftSpeed), 0, 100, 1, 10);  // Scale to control line speed
        int rightMotorSpeed = (int) map(abs(rightSpeed), 0, 100, 1, 10);

        // Move the lines
        if (leftSpeed > 0) {
            lineYLeft -= leftMotorSpeed;  // Move up when speed is positive
        } else if (leftSpeed < 0) {
            lineYLeft += leftMotorSpeed;  // Move down when speed is negative
        }

        if (rightSpeed > 0) {
            lineYRight -= rightMotorSpeed;  // Move up when speed is positive
        } else if (rightSpeed < 0) {
            lineYRight += rightMotorSpeed;  // Move down when speed is negative
        }

        // Reset line positions when they exit the top or bottom of the rectangle
        if (lineYLeft < 150) lineYLeft = 150 + RECT_HEIGHT_PIXELS;  // Wrap from top to bottom
        if (lineYLeft > 150 + RECT_HEIGHT_PIXELS) lineYLeft = 150;  // Wrap from bottom to top

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
