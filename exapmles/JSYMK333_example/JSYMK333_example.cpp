#include <Arduino.h>
#include "JSYMK333.h"

#define RX_PIN 16
#define TX_PIN 17

JSYMK333 meter(Serial2, 9600, SERIAL_8N1, RX_PIN, TX_PIN);

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
    Serial.print("A phase voltage: ");
    Serial.print(meter.readVoltageA());
    Serial.println(" V");

    Serial.print("B phase voltage: ");
    Serial.print(meter.readVoltageB());
    Serial.println(" V");

    Serial.print("C phase voltage: ");
    Serial.print(meter.readVoltageC());
    Serial.println(" V");

    delay(1000); // read every second
}
