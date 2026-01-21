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

Working with W25Q128 and 25LC040A memory can differenciate here
|Metric|W25Q128|25LC040A|
|---|---|---|
|Capacity|16 MB|512 bytes|
|Writing|By pages|By bytes|
|Erasing before writing|Required|Not required|