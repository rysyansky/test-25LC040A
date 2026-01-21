### Test project

This project is reader/writer class for 25LC040A EEPROM microchip

Documented with doxygen-style comments. Full dcumentation is provided in ```./docs``` directory

Tested project for build errors by:
```
g++ -std=c++17 \
    -I. \
    eeprom/Eeprom25LC040A.cpp \
    abstract-spibb/MockSPIBitBanger.cpp \ 
    main.cpp \
    -o test_eeprom
```
Project was build on Arch Linux with gcc.

