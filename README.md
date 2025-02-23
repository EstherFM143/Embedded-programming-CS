# Computer-Systems
Morse Code Communication Device using SensorTag

Project in Collaboration with @Sofyaiv

Overview

This project develops a communication device using the SensorTag board, enabling agents to send and receive secret messages encoded in Morse code. The device uses the MPU9250 sensor to detect specific movements or button presses, translating them into Morse code symbols (dots, dashes, and spaces). These symbols are transmitted to a workstation via UART, where they are decoded into text. The device can also receive Morse-coded messages and present them to the user using LEDs or a buzzer.

Key Features

Morse Code Encoding/Decoding: Converts movements or button presses into Morse code and decodes received messages.
Sensor Integration: Uses the MPU9250 accelerometer and gyroscope to detect movements or positions.
UART Communication: Sends and receives Morse code symbols via UART, with each symbol ending in a newline character.
User Interface: Includes buttons, LEDs, and sounds for user feedback.
Real-Time Operating System (RTOS): Built on TI-RTOS for task-based execution and real-time processing.

Requirements

Movement Detection: Recognize at least two distinct movements or positions using the MPU9250 sensor.
Task-Based Implementation: Divide the program into tasks (e.g., sensor data collection, Morse encoding, UART communication).
User Feedback: Provide visual (LEDs) or auditory (buzzer) feedback for recognized commands and incoming messages.
State Machine: Use of a state machine to manage program logic and transitions.
Morse Translation: Add functionality to translate Morse code to text and vice versa.
Error Handling: Implement thresholds and advanced algorithms for movement recognition.

Tools and Libraries

TI-RTOS: For task management.
MPU9250 Sensor: For movement and position detection.
UART: For serial communication.
Serial Client: Pre-installed program for Morse code communication on the workstation.
