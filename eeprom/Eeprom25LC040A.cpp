#include "Eeprom25LC040A.hpp"
#include "../abstract-spibb/SPIBitBanger.hpp"
#include <cassert>
#include <cstdint>

void Eeprom25LC040A::setWriteLatch() {
    spi.startTransaction();
    spi.sendData(WREN);
    spi.endTransaction();
}

void Eeprom25LC040A::resetWriteLatch() {
    spi.startTransaction();
    spi.sendData(WRDI);
    spi.endTransaction();
}

void Eeprom25LC040A::waitUntilWriteIsComplete() {
    uint8_t status;
    while (status != 0x01) {
        spi.startTransaction();
        spi.sendData(RDSR);
        status = spi.getData();
        spi.endTransaction();
    }
}

void Eeprom25LC040A::splitInto2AddressBytes(uint16_t address, uint8_t command, uint8_t* commandbyte, uint8_t* addressbyte) {
    assert(address <= 0x01FF && "Address must be lesser or equal then 9 bits!");
    uint8_t a8 = (address >> 8) & 0x01;
    *commandbyte = (a8 << 3) | command;
    *addressbyte = address & 0xFF;
}

uint8_t Eeprom25LC040A::readByte(uint16_t address) {
    assert(address <= 0x01FF && "Address must be lesser or equal then 9 bits!");
    uint8_t databyte, commandbyte, addressbyte;
    splitInto2AddressBytes(address, READ, &commandbyte, &addressbyte);
    spi.startTransaction();
    spi.sendData(commandbyte);
    spi.sendData(addressbyte);
    databyte = spi.getData();
    spi.endTransaction();
    return databyte;
}

uint8_t* Eeprom25LC040A::readData(uint16_t address, uint8_t len = 1) {
    assert(address <= 0x01FF && "Address must be lesser or equal then 9 bits!");
    assert(len <= 512 - address && "Length of reading must address the range: [address; 512]");
    uint8_t* data{new uint8_t{len}};
    for(uint16_t i = 0; i < len; i++) {
        data[i] = readByte(address + i);
    }
    return data;
}

void Eeprom25LC040A::writeData(uint16_t address, const uint8_t* data_buffer, uint8_t len = 1) {
    assert(address <= 0x01FF && "Address must be lesser or equal then 9 bits!");
    assert(len <= 512 - address && "Length of writing must address the range: [address; 512]");
    uint8_t commandbyte, addressbyte;
    splitInto2AddressBytes(address, WRITE, &commandbyte, &addressbyte);
    setWriteLatch();
    spi.startTransaction();
    spi.sendData(commandbyte);
    spi.sendData(addressbyte);
    for(uint16_t i = 0; i < len; i++) {
        spi.sendData(data_buffer[i]);
    }
    spi.endTransaction();
    waitUntilWriteIsComplete();
}