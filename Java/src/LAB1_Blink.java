import org.firmata4j.IODevice;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;

import java.io.IOException;

public class LAB1_Blink {

    public static void main(String[] args) throws IOException, InterruptedException {
        // Replace "COM3" with the port your Arduino is connected to (e.g., "COM3" on Windows, "/dev/ttyUSB0" on Linux)
        String portName = "COM13";

        // Initialize the Firmata device
        IODevice arduino = new FirmataDevice(portName);

        // Start communication with the Arduino
        arduino.start();
        arduino.ensureInitializationIsDone();
        System.out.println("Arduino initialized!");

        // Get pin 13 (where the LED is connected)
        Pin ledPin = arduino.getPin(13);
        ledPin.setMode(Pin.Mode.OUTPUT);  // Set pin 13 as output

        // Blink the LED
        while (true) {
            // Turn the LED ON
            ledPin.setValue(1);
            System.out.println("LED ON");
            Thread.sleep(1000);  // Wait for 500 milliseconds

            // Turn the LED OFF
            ledPin.setValue(0);
            System.out.println("LED OFF");
            Thread.sleep(1000);  // Wait for 500 milliseconds
        }
    }
}

