# JSYMK333

A C++ library for the JSY-MK-333 three-phase embedded meter module using the Modbus RTU protocol for ESP32.

## Features

- Read various electrical parameters such as voltage, current, power, energy, and more.
- Simple and easy-to-use API.

## Installation

### Requirements

- `ModbusRTU` library for Arduino.

### Building

Copy the `JSYMK333` folder to your Arduino `libraries` folder.

## Usage

See the example in `examples/JSYMK333_example/JSYMK333_example.cpp`:

```cpp
#include <Arduino.h>
#include "JSYMK333.h"

#define RX_PIN 16
#define TX_PIN 17

JSYMK333 meter(Serial2, 9600, SERIAL_8N1, RX_PIN, TX_PIN);

unsigned long previousMillis = 0;
const long interval = 1000; // interval at which to read values (milliseconds)

void setup() {
    Serial.begin(115200);
    while (!Serial) { ; } // wait for serial port to connect

    if (!meter.begin()) {
        Serial.println("Failed to initialize JSYMK333 meter");
        while (1) { ; } // halt
    }

    Serial.println("JSYMK333 meter initialized");
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        Serial.print("A phase voltage: ");
        Serial.print(meter.readVoltageA());
        Serial.println(" V");

        Serial.print("B phase voltage: ");
        Serial.print(meter.readVoltageB());
        Serial.println(" V");

        Serial.print("C phase voltage: ");
        Serial.print(meter.readVoltageC());
        Serial.println(" V");
    }
}
