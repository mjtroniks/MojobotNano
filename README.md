# Mojobot Nano BLE Robot
![Mojobot nano](https://github.com/user-attachments/assets/35aa67c4-03c1-46ed-8880-0a4dccc69f64)

Welcome to the official repository for the Mojobot Nano BLE Robot! This project uses the Arduino Nano BLE to control a robot equipped with infrared sensors for path tracking, ultrasonic sensors for obstacle avoidance, and LEDs for status indication. Enjoy the added functionality of Bluetooth Low Energy (BLE) for remote control and monitoring.

## Features

- **Path Tracking:** Utilizes infrared sensors to follow a designated path.
- **Obstacle Avoidance:** Employs an ultrasonic sensor to detect and avoid obstacles.
- **LED Indicators:** Provides visual feedback through LEDs.
- **BLE Control:** Control and monitor the robot remotely via Bluetooth.

## Hardware Components

- Arduino Nano BLE
- Infrared Sensors:
  - **Left:** Pin D5
  - **Right:** Pin D4
- Motors:
  - **Left Motor PWM:** D7
  - **Right Motor PWM:** D6
  - **Left Motor Direction:** D8
  - **Right Motor Direction:** D9
- LEDs:
  - **Left Red LED:** A2
  - **Left Blue LED:** A3
  - **Left Green LED:** D10
  - **Right Red LED:** D11
  - **Right Blue LED:** A6
  - **Right Green LED:** A7
- Ultrasonic Sensor:
  - **Trigger Pin:** D2
  - **Echo Pin:** D3

## Setup

1. **Assembly:** Connect all components as described in the Hardware Components section.
2. **Programming:** Upload the code from this repository to your Arduino Nano BLE using the Arduino IDE.
3. **Testing:** Power the Arduino and test the BLE functionality with a compatible smartphone or computer.

## Usage

Once set up, the robot can be controlled via a BLE connection. Use any standard BLE app to connect to the robot. The app will allow you to:
- Start or stop the robot.
- Change the robot's speed and direction.
- Monitor sensor readings.

## Development

This project is open to contributions! To contribute:
1. Fork the repository.
2. Create a feature branch: `git checkout -b new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin new-feature`
5. Submit a pull request.

## Issues

For any issues or bugs, please file an issue on GitHub, providing as much detail as possible about the problem and how it can be replicated.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Authors

- **Your Name** - *Initial work* - [YourGitHub](https://github.com/YourGitHub)

## Acknowledgments

- Hat tip to anyone whose code was used
- Inspiration
- etc
