#include "JSYMK333.h"

JSYMK333::JSYMK333(HardwareSerial& serial, int baud, uint8_t config, int rxPin, int txPin)
    : serial(serial), baud(baud), config(config), rxPin(rxPin), txPin(txPin) {}

JSYMK333::~JSYMK333() {}

bool JSYMK333::begin() {
    serial.begin(baud, config, rxPin, txPin);
    return mb.begin(&serial);
}

float JSYMK333::readSingleRegister(uint16_t address, float factor) {
    uint16_t reg;
    if (mb.readHreg(1, address, &reg, 1) == mb.ku8MBSuccess) {
        return reg / factor;
    } else {
        return NAN;
    }
}

float JSYMK333::readDoubleRegister(uint16_t address, float factor) {
    uint16_t regs[2];
    if (mb.readHreg(1, address, regs, 2) == mb.ku8MBSuccess) {
        uint32_t value = (regs[0] << 16) | regs[1];
        return value / factor;
    } else {
        return NAN;
    }
}

float JSYMK333::readVoltageA() { return readSingleRegister(0x0100, 100.0); }
float JSYMK333::readVoltageB() { return readSingleRegister(0x0101, 100.0); }
float JSYMK333::readVoltageC() { return readSingleRegister(0x0102, 100.0); }
float JSYMK333::readCurrentA() { return readSingleRegister(0x0103, 100.0); }
float JSYMK333::readCurrentB() { return readSingleRegister(0x0104, 100.0); }
float JSYMK333::readCurrentC() { return readSingleRegister(0x0105, 100.0); }
float JSYMK333::readActivePowerA() { return readSingleRegister(0x0106, 1.0); }
float JSYMK333::readActivePowerB() { return readSingleRegister(0x0107, 1.0); }
float JSYMK333::readActivePowerC() { return readSingleRegister(0x0108, 1.0); }
float JSYMK333::readTotalActivePower() { return readDoubleRegister(0x0109, 1.0); }
float JSYMK333::readReactivePowerA() { return readSingleRegister(0x010B, 1.0); }
float JSYMK333::readReactivePowerB() { return readSingleRegister(0x010C, 1.0); }
float JSYMK333::readReactivePowerC() { return readSingleRegister(0x010D, 1.0); }
float JSYMK333::readTotalReactivePower() { return readDoubleRegister(0x010E, 1.0); }
float JSYMK333::readApparentPowerA() { return readSingleRegister(0x0110, 1.0); }
float JSYMK333::readApparentPowerB() { return readSingleRegister(0x0111, 1.0); }
float JSYMK333::readApparentPowerC() { return readSingleRegister(0x0112, 1.0); }
float JSYMK333::readTotalApparentPower() { return readDoubleRegister(0x0113, 1.0); }
float JSYMK333::readFrequency() { return readSingleRegister(0x0115, 100.0); }
float JSYMK333::readPowerFactorA() { return readSingleRegister(0x0116, 1000.0); }
float JSYMK333::readPowerFactorB() { return readSingleRegister(0x0117, 1000.0); }
float JSYMK333::readPowerFactorC() { return readSingleRegister(0x0118, 1000.0); }
float JSYMK333::readTotalPowerFactor() { return readSingleRegister(0x0119, 1000.0); }
float JSYMK333::readActiveEnergyA() { return readDoubleRegister(0x011A, 100.0); }
float JSYMK333::readActiveEnergyB() { return readDoubleRegister(0x011C, 100.0); }
float JSYMK333::readActiveEnergyC() { return readDoubleRegister(0x011E, 100.0); }
float JSYMK333::readTotalActiveEnergy() { return readDoubleRegister(0x0120, 100.0); }
float JSYMK333::readReactiveEnergyA() { return readDoubleRegister(0x0122, 100.0); }
float JSYMK333::readReactiveEnergyB() { return readDoubleRegister(0x0124, 100.0); }
float JSYMK333::readReactiveEnergyC() { return readDoubleRegister(0x0126, 100.0); }
float JSYMK333::readTotalReactiveEnergy() { return readDoubleRegister(0x0128, 100.0); }
float JSYMK333::readApparentEnergyA() { return readDoubleRegister(0x012A, 100.0); }
float JSYMK333::readApparentEnergyB() { return readDoubleRegister(0x012C, 100.0); }
float JSYMK333::readApparentEnergyC() { return readDoubleRegister(0x012E, 100.0); }
float JSYMK333::readTotalApparentEnergy() { return readDoubleRegister(0x0130, 100.0); }
uint16_t JSYMK333::readPowerDirection() { return readSingleRegister(0x0132, 1.0); }
uint16_t JSYMK333::readAlarmStatus() { return readSingleRegister(0x0133, 1.0); }
