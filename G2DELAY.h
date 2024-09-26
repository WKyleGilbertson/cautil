// Filename : G2DELAY.h
#pragma once
class G2DELAY {
  public:
    char CACODE[1023];
    char CODE[1023];
    unsigned short G2a[1023], G1a[1023];
    unsigned short F10, F10I;
    unsigned char prn;
    int cphase;
    G2DELAY(int PRN, int CODEPHASE);
    int PRNGEN(char * CACODE, int PRN, int CODEPHASE);
    int DSPCODE(char * CODE, char * CACODE);
};
