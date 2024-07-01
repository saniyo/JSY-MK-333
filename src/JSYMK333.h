#ifndef JSYMK333_H
#define JSYMK333_H

#include <Arduino.h>
#include <HardwareSerial.h>

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
    HardwareSerial& serial;
    int baud;
    uint8_t config;
    int rxPin;
    int txPin;

    float readSingleRegister(uint16_t address, float factor);
    float readDoubleRegister(uint16_t address, float factor);

    bool sendCmd8(uint8_t cmd, uint16_t rAddr, uint16_t val, bool check, uint16_t slave_addr = 0xFFFF);
    uint16_t receive(uint8_t *resp, uint16_t len);
    bool checkCRC(const uint8_t *buf, uint16_t len);
    void setCRC(uint8_t *buf, uint16_t len);
    uint16_t CRC16(const uint8_t *data, uint16_t len);
};

#endif // JSYMK333_H
