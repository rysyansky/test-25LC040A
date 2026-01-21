#pragma once

#include <cstdint>
class SPIBitBanger {
public:
    void startTransaction();
    void endTransaction();
    void sendData(uint8_t data);
    uint8_t getData();
};