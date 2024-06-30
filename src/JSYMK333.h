#ifndef JSYMK333_H
#define JSYMK333_H

#include <Arduino.h>
#include <ModbusRTU.h>

class JSYMK333 {
public:
    JSYMK333(HardwareSerial& serial, int baud, uint8_t config, int rxPin, int txPin);
    ~JSYMK333();

    bool begin();
    float readVoltageA();
    float readVoltageB();
    float readVoltageC();
    float readCurrentA();
    float readCurrentB();
    float readCurrentC();
    float readActivePowerA();
    float readActivePowerB();
    float readActivePowerC();
    float readTotalActivePower();
    float readReactivePowerA();
    float readReactivePowerB();
    float readReactivePowerC();
    float readTotalReactivePower();
    float readApparentPowerA();
    float readApparentPowerB();
    float readApparentPowerC();
    float readTotalApparentPower();
    float readFrequency();
    float readPowerFactorA();
    float readPowerFactorB();
    float readPowerFactorC();
    float readTotalPowerFactor();
    float readActiveEnergyA();
    float readActiveEnergyB();
    float readActiveEnergyC();
    float readTotalActiveEnergy();
    float readReactiveEnergyA();
    float readReactiveEnergyB();
    float readReactiveEnergyC();
    float readTotalReactiveEnergy();
    float readApparentEnergyA();
    float readApparentEnergyB();
    float readApparentEnergyC();
    float readTotalApparentEnergy();
    uint16_t readPowerDirection();
    uint16_t readAlarmStatus();

private:
    ModbusRTU mb;
    HardwareSerial& serial;
    int baud;
    uint8_t config;
    int rxPin;
    int txPin;

    float readSingleRegister(uint16_t address, float factor);
    float readDoubleRegister(uint16_t address, float factor);
};

#endif // JSYMK333_H
