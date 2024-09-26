// Filename : G2INIT.h
#pragma once
#include <cstdint>
class G2INIT {
  public:
    char CACODE[1023];
    char CODE[1023];
    unsigned short G2a[1023], G1a[1023];
    unsigned short F10, F10I;
    unsigned char prn;
    int cphase;
    int PRNGEN(char * CACODE, uint8_t PRN, uint16_t CODEPHASE);
    int DSPCODE(char *CODE, char *CACODE);
    G2INIT(uint8_t PRN, uint16_t CODEPHASE);
};
