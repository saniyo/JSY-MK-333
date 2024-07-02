#include "JSYMK333.h"

JSYMK333::JSYMK333(HardwareSerial& serial, int baud, uint8_t config, int rxPin, int txPin)
    : serial(serial), baud(baud), config(config), rxPin(rxPin), txPin(txPin) {}

JSYMK333::~JSYMK333() {}

bool JSYMK333::begin() {
    serial.begin(baud, config, rxPin, txPin);
    // Custom initialization if needed
    return true;
}

float JSYMK333::readSingleRegister(uint16_t address, float factor) {
    uint16_t reg;
    if (sendCmd8(0x03, address, 1, true) && receive((uint8_t*)&reg, 2) == 2) {
        return reg / factor;
    } else {
        return NAN;
    }
}

float JSYMK333::readDoubleRegister(uint16_t address, float factor) {
    uint16_t regs[2];
    if (sendCmd8(0x03, address, 2, true) && receive((uint8_t*)regs, 4) == 4) {
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

bool JSYMK333::sendCmd8(uint8_t cmd, uint16_t rAddr, uint16_t val, bool check, uint16_t slave_addr) {
    uint8_t sendBuffer[8];
    uint8_t respBuffer[8];

    if ((slave_addr == 0xFFFF) || (slave_addr < 0x01) || (slave_addr > 0xF7))
        slave_addr = 1; // Default address

    sendBuffer[0] = slave_addr;
    sendBuffer[1] = cmd;
    sendBuffer[2] = (rAddr >> 8) & 0xFF;
    sendBuffer[3] = (rAddr) & 0xFF;
    sendBuffer[4] = (val >> 8) & 0xFF;
    sendBuffer[5] = (val) & 0xFF;
    setCRC(sendBuffer, 8);

    serial.write(sendBuffer, 8);

    if (check) {
        if (receive(respBuffer, 8) != 8)
            return false;
        for (uint8_t i = 0; i < 8; i++) {
            if (sendBuffer[i] != respBuffer[i])
                return false;
        }
    }
    return true;
}

uint16_t JSYMK333::receive(uint8_t *resp, uint16_t len) {
    unsigned long startTime = millis();
    uint8_t index = 0;
    while ((index < len) && (millis() - startTime < 500)) {
        if (serial.available() > 0) {
            uint8_t c = (uint8_t)serial.read();
            resp[index++] = c;
        }
        yield();
    }

    if (!checkCRC(resp, index)) {
        return 0;
    }
    return index;
}

bool JSYMK333::checkCRC(const uint8_t *buf, uint16_t len) {
    if (len <= 2)
        return false;

    uint16_t crc = CRC16(buf, len - 2);
    return ((uint16_t)buf[len - 2] | (uint16_t)buf[len - 1] << 8) == crc;
}

void JSYMK333::setCRC(uint8_t *buf, uint16_t len) {
    if (len <= 2)
        return;

    uint16_t crc = CRC16(buf, len - 2);

    buf[len - 2] = crc & 0xFF;
    buf[len - 1] = (crc >> 8) & 0xFF;
}

uint16_t JSYMK333::CRC16(const uint8_t *data, uint16_t len) {
    static const uint16_t crcTable[] PROGMEM = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

    uint8_t nTemp;
    uint16_t crc = 0xFFFF;

    while (len--) {
        nTemp = *data++ ^ crc;
        crc >>= 8;
        crc ^= pgm_read_word(&crcTable[nTemp]);
    }
    return crc;
}
