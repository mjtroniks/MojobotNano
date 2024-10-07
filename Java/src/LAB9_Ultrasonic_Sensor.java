import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import processing.core.PApplet;

public class LAB9_Ultrasonic_Sensor extends PApplet {

    // Declare the ultrasonic sensor pins
    private Pin triggerPin, echoPin;
    private IODevice arduino;

    float distance_cm = 0;
    float max_distance_cm = 300; // 3 meters is usually the max range of the sensor

    public static void main(String[] args) {
        PApplet.main("LAB9_Ultrasonic_Sensor");
    }

    public void settings() {
        size(600, 600);  // Window size
    }

    public void setup() {
        // Set up the Arduino connection
        String portName = "COM13";  // Adjust this to your port
        try {
            arduino = new FirmataDevice(portName);
            arduino.start();
            arduino.ensureInitializationIsDone();

            // Initialize the trigger and echo pins
            triggerPin = arduino.getPin(2);
            echoPin = arduino.getPin(3);
            triggerPin.setMode(Pin.Mode.OUTPUT);
            echoPin.setMode(Pin.Mode.INPUT);

            println("Arduino initialized!");

        } catch (Exception e) {
            e.printStackTrace();
            exit();
        }
    }

    public void draw() {
        background(255);  // White background
        measureDistance();  // Measure distance using the sensor

        // Simulate arcs and line
        drawDistanceArcs(distance_cm);
        drawDistanceLine(distance_cm);

        // Display distance text
        fill(0);
        textSize(20);
        textAlign(CENTER);
        text("Distance: " + nf(distance_cm, 0, 2) + " cm", width / 2, height - 50);
    }

    // Measure the distance using the ultrasonic sensor
    public void measureDistance() {
        try {
            // Trigger the ultrasonic pulse
            triggerPin.setValue(0);
            delayMicroseconds(2);
            triggerPin.setValue(1);
            delayMicroseconds(10);
            triggerPin.setValue(0);

            // Read the echo pin
            long duration = pulseIn(echoPin, true, 30000); // 30ms timeout for max range

            // Calculate distance in centimeters
            distance_cm = (float)(duration * 0.034 / 2);  // Fix: Explicitly cast to float
        } catch (Exception e) {
            println("Error measuring distance");
            e.printStackTrace();
        }
    }

    // Draw three arcs at different ranges to simulate sound waves
    public void drawDistanceArcs(float distance) {
        stroke(0, 100);  // Semi-transparent black for arcs
        noFill();

        // Explicitly cast the result of the calculation to float
        float arc_radius = (float) map(distance, 0, max_distance_cm, 50, 200);

        for (int i = 1; i <= 3; i++) {
            arc(width / 2, height / 2 + 100, arc_radius * i, arc_radius * i, PI, TWO_PI);
        }
    }

    // Draw the line extending outwards based on distance
    public void drawDistanceLine(float distance) {
        float max_line_length = 300;  // Max length of the line
        // Explicitly cast the result of the calculation to float
        float line_length = (float) map(distance, 0, 100, 0, max_line_length);  // Max line length is 10 cm in simulation

        stroke(0);  // Black line
        strokeWeight(3);
        line(width / 2, height / 2 + 100, width / 2, height / 2 + 100 - line_length);
    }


    // Simulate the Arduino delayMicroseconds function
    public void delayMicroseconds(int microseconds) {
        long start = System.nanoTime();
        while (System.nanoTime() - start < microseconds * 1000) {
            // busy-waiting for the delay to complete
        }
    }

    // Function to simulate pulseIn function in Firmata (this is a basic version)
    public long pulseIn(Pin pin, boolean value, long timeout) {
        long startTime = System.nanoTime();
        while (pin.getValue() != (value ? 1 : 0)) {
            if (System.nanoTime() - startTime > timeout * 1000) {
                return 0; // Timeout
            }
        }

        long pulseStart = System.nanoTime();
        while (pin.getValue() == (value ? 1 : 0)) {
            if (System.nanoTime() - pulseStart > timeout * 1000) {
                return 0; // Timeout
            }
        }

        return (System.nanoTime() - pulseStart) / 1000;  // Return pulse duration in microseconds
    }
}
