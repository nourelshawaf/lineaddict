# 5-Channel Line Follower Robot

## Overview

This project is a 5-channel IR sensor based line follower robot controlled by an Arduino Nano. The robot detects a black line on a white surface and adjusts the speed of two DC gear motors through an L298N motor driver.

The final version uses a separated power system: a boost converter provides 12V for the motors through the L298N, while a buck converter provides stable 5V for the Arduino Nano and IR sensor array.

## Project Preview

![Robot Top View](images/robot_top_view.jpg)

## System Architecture

![System Architecture](diagrams/system_architecture.png)

## Main Features

- 5-channel IR line detection
- Arduino Nano based control
- L298N motor driver
- Two DC gear motors
- Separate 12V motor supply and 5V logic supply
- PID-based motor correction
- Common-ground power design

## Hardware Used

| Component | Purpose |
|---|---|
| Arduino Nano | Main controller |
| 5-Channel IR Sensor Array | Line detection |
| L298N Motor Driver | Motor control |
| Two DC Gear Motors | Robot movement |
| Boost Converter | 12V motor power |
| Buck Converter | 5V logic power |
| Battery Pack | Main power source |

Full list: [Components Documentation](docs/components.md)

## Wiring Diagram

![Fritzing Wiring Diagram](diagrams/fritzing_wiring.png)

Detailed wiring: [Wiring Documentation](docs/wiring.md)

## Control Logic

The robot reads the five IR sensors from A1 to A5. Each sensor represents a position relative to the center of the line. The Arduino calculates the line error and adjusts the left and right motor speeds through the L298N motor driver.

Read more: [Control Logic](docs/control_logic.md)

## Code

Main Arduino code:

[View Code](code/line_follower_pid.ino)

## Testing

The robot was tested on straight lines, curves, and sharp turns. Motor direction, sensor response, and power stability were tested separately before full line-following tests.

Testing notes: [Testing Documentation](docs/testing.md)

## Problems and Solutions

The main issue during development was unstable motor movement caused by power delivery problems. This was improved by separating motor power and logic power using boost and buck converters.

Read more: [Problems and Solutions](docs/problems_and_solutions.md)

## Demo Video

Demo link: [Watch Demo](videos/demo_video.md)

## Author

Noureldeen Elshawaf  
Mechatronics Engineering Student  
University of Debrecen