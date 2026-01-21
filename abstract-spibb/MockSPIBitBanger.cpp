#include "SPIBitBanger.hpp"
#include <iostream>

void SPIBitBanger::startTransaction() {}

void SPIBitBanger::endTransaction() {}

void SPIBitBanger::sendData(uint8_t data) {}

uint8_t SPIBitBanger::getData() {
    return 0xFF;
}