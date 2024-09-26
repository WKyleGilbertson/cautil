// Filename : G2INIT.h
#pragma once
#include <cstdint>
class G2INIT {
  public:
    uint8_t CACODE[1023];
    int8_t  CODE[1023];
    uint16_t G2a[1023], G1a[1023];
    uint16_t F10, F10I;
    uint8_t  prn;
    uint16_t cphase;
    int PRNGEN(uint8_t *CACODE, uint8_t PRN, uint16_t CODEPHASE);
    int DSPCODE(int8_t *CODE, uint8_t *CACODE);
    G2INIT(uint8_t PRN, uint16_t CODEPHASE);
};
