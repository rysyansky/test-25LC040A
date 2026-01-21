

#pragma once

#include <cstdint>

class SPIBitBanger;

/** 
    \brief Class for working with data on 25LC040A EEPROM
    \details This class provides reading and writing functionality for 25LC040A microchip by abstract driver SPIBitBanger.
    SPIBitBanger is considered to be capable of setting CS, WP pins high and low, synchronising communication, reading from SO and writing to SI.
    \warning This class do not provide hyper-extencive error-handling. All passed data is considered to be valid.
*/
class Eeprom25LC040A {
private:
    /// SPI-driver class
    SPIBitBanger& spi;
    /// \brief Commands to operate microchip
    enum Commands : uint8_t {
        READ = 0x03, ///< Read data
        WRITE = 0x02,///< Write data
        WRDI = 0x04, ///< Resets the enable write latch
        WREN = 0x06, ///< Sets the enable write latch
        RDSR = 0x05, ///< Read STATUS register
        WRSR = 0x01  ///< Write to STATUS register
    };
    /// \brief Sets write latch
    /// \details This function used for enabling writing by sending WREN to microchip to set write latch
    void setWriteLatch();
    /** 
        \brief Resets write latch
        \details This function used for disabling writing by sending WRDI to microchip to reset write latch
        \warning This function is not used at the moment because WRITE resets write latch as default from information provided in datasheet
    */
    void resetWriteLatch();
    /** 
        \brief Wait until STATUS register is 0x01
        \details This function is used to wait until writing is complete by checking STATUS register
        \warning This function is required after each write operation to provide correctness and stability, if STATUS is not 0x01 next writing operation could be skipped 
    */
    void waitUntilWriteIsComplete();
    /** 
        \brief Splits 9bit address into 2 required addresses
        \details Splits 9bit address into two 8bit sequences. Commands READ and WRITE requires sending two addresses as [0000 a8c2c1c0][a7-a0]
        a8 is 9th bit of 9bit address - [000a8 a7-a0], it is used to address full 512 byte array
        c2-c0 is command reserved for READ or WRITE command
        \param address 9bit address
        \param command WRITE or READ command
        \param commandbyte Pointer to desired variable for command byte
        \param addressbyte Pointer to desired variable for 8bit address byte
    */
    void splitInto2AddressBytes(uint16_t address, uint8_t command, uint8_t* commandbyte, uint8_t* addressbyte);
public:
    /**
        \brief Constructor for Eeprom25LC040A class
        \param spi SPI driver instance
    */
    Eeprom25LC040A(SPIBitBanger& spi) : spi(spi) {};
    /**
        \brief Read one byte of data
        \param address 9bit address of a byte
        \return Byte of data
    */
    uint8_t readByte(uint16_t address);
    /**
        \brief Read sequence of bytes
        \details This function is a wrapper for readByte, that read bytes in a cycle
        \param address 9bit address of a byte
        \param len Length of readable sequence, 1 by default
        \return Pointer to readed sequence
    */
    uint8_t* readData(uint16_t address, uint8_t len);
    /**
        \brief Write sequence of data
        \details This functions provides writing data to microchip. It can be used to write either single byte (2-2 in datasheet) or a whole page (2-3 in datasheet)
        \param address 9bit address of a byte
        \param data_buffer Pointer to a sequence of writable bytes
        \param len Length of readable sequence, 1 by default
    */
    void writeData(uint16_t address, const uint8_t* data_buffer, uint8_t len);
};