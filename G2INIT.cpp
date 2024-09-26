// Filename : G2INIT.cpp
using namespace std;
#include <cstdint>
#include <cstdio>
#include "G2INIT.h"

#define TAP1    0x0200
#define TAP2    0x0100
#define TAP3    0x0080
#define TAP6    0x0010
#define TAP8    0x0004
#define TAP9    0x0002
#define TAP10   0x0001

uint16_t flip(uint16_t number) {
 uint16_t retVal;
   retVal  = ((number&0x0001)<<9) | ((number&0x0002)<<7) | ((number&0x0004)<<5)
          | ((number&0x0008)<<3) | ((number&0x0010)<<1) | ((number&0x0020)>>1)
          | ((number&0x0040)>>3) | ((number&0x0080)>>5) | ((number&0x0100)>>7)
          | ((number&0x0200)>>9);
 return retVal;
}

G2INIT::G2INIT(uint8_t PRN, uint16_t CODEPHASE) {
cphase = CODEPHASE % 1023;
prn = PRN;
PRNGEN(CACODE, prn, cphase);
DSPCODE(CODE, CACODE);
}

int G2INIT::PRNGEN(uint8_t * CACODE, uint8_t PRN, uint16_t CODEPHASE) {
  //int main(int argc, char * argv[]) {
//  if (argc < 2) exit(1);
//  unsigned short PRN = atoi(argv[1]);
  /* Table A-1 contains PRN number and G2 delay / Initial G2 and 1st 10 chips*/
  /* IS-GPS-200 provides delay and First 10 CA chips which is (G2init^0x3FF)*/
  unsigned short GPSDelays[37] = {5, 6, 7, 8, 17, 18, 139, 140, 141, 251, 252,
  254, 255, 256, 257, 258, 469, 470, 471, 472, 473, 474, 509, 512, 513, 514,
  515, 516, 859, 860, 861, 862, 863, 950, 947, 948, 950};
  unsigned short GPSF10[37] = {01440, 01620, 01710, 01744, 01133, 01455,
    01131, 01454, 01626, 01504, 01642, 01750, 01764, 01772, 01775, 01776,
    01156, 01467, 01633, 01715, 01746, 01763, 01063, 01706, 01743, 01761, 01770,
    01774, 01127, 01453, 01625, 01712, 01745, 01713, 01134, 01456, 01713};
  unsigned short SBASDelays[39] = {145, 175, 52, 21, 237, 235, 886, 657, 634,
    762, 355, 1012, 176, 603, 130, 359, 595, 68, 386, 797, 456, 499, 883, 307,
    127, 211, 121, 118, 163, 628, 853, 484, 289, 811, 202, 1021, 463, 568, 904};
  unsigned short SBASF10[39] = {00671, 00536, 01510, 01545, 00160, 00701,
    00013, 01060, 00245, 00527, 01436, 01226, 01257, 00046, 01071, 00561,
    01037, 00770, 01327, 01472, 00124, 00366, 00133, 00465, 00717, 00217,
    01742, 01422, 01442, 00523, 00736, 01635, 00136, 00273, 01026, 00003};
  unsigned short G1=0x03FF;
  unsigned short G2=0x03FF; // G2=0x0258; Octal Inverse of o1626, or first ten chips? PRN9
  signed char CA[1023];
  unsigned short G1FeedBackBit = 0x0000, G2FeedBackBit = 0x0000;
  unsigned short F10Chips = 0x0000, F10Inverse = 0x0000, CAreg = 0;
  short int idx = 0;

  G2 = (PRN<40) ? (flip(GPSF10[PRN-1]^0x3FF)) : (flip(SBASF10[PRN-120]^0x3FF));
//  printf("G2Init: o%.4o F10: o%.4o \n", G2, flip(G2)^0x3FF);
  
   for (idx=0; idx<1033; idx++) {
     CAreg = CAreg >> 1;
     CA[idx%1023] = ((G1 & TAP10) ^ (G2 & TAP10))  ? 1 : 0;
     CAreg |= ((CA[idx%1023]) & 1) << 9;

     G1a[idx%1023] = G1;
     G1FeedBackBit = ((G1 & TAP3) >> 7) ^ (G1 & TAP10);
     G1 |= (G1FeedBackBit << 10);
     G1 = G1 >> 1;

     G2a[idx%1023] = G2;
     G2FeedBackBit =
      ((G2 & TAP2) >> 8) ^ ((G2 & TAP3) >> 7) ^ ((G2 & TAP6) >> 4) ^
      ((G2 & TAP8) >> 2) ^ ((G2 & TAP9) >> 1) ^ (G2 & TAP10);
     G2 |= (G2FeedBackBit << 10);
     G2 = G2 >> 1;
   }

  for (idx=0; idx<1023; idx++) {
    CACODE[idx] = CA[(idx+CODEPHASE) % 1023];
  }

  F10Chips = (CA[0]<<9) | (CA[1]<<8) | (CA[2]<<7) | (CA[3]<<6) | (CA[4]<<5) |
           (CA[5]<<4) | (CA[6]<<3) | (CA[7]<<2) | (CA[8]<<1) | CA[9];
  F10 = F10Chips;
  F10Inverse = (CA[0]<<0) | (CA[1]<<1) | (CA[2]<<2) | (CA[3]<<3) | (CA[4]<<4) |
           (CA[5]<<5) | (CA[6]<<6) | (CA[7]<<7) | (CA[8]<<8) | (CA[9]<<9);
  F10I = F10Inverse; /*
  F10Inverse = (CA[0]<<9) | (CA[1]<<8) | (CA[2]<<7) | (CA[3]<<6) | (CA[4]<<5) |
           (CA[5]<<4) | (CA[6]<<3) | (CA[7]<<2) | (CA[8]<<1) | CA[9];
  F10Chips = (CA[0]<<0) | (CA[1]<<1) | (CA[2]<<2) | (CA[3]<<3) | (CA[4]<<4) |
           (CA[5]<<5) | (CA[6]<<6) | (CA[7]<<7) | (CA[8]<<8) | (CA[9]<<9); */
//printf("First 10: o%.4o F10 Inv: o%.4o\n", F10Chips, F10Inverse);
//printf("%o_%o%o%o_%o%o%o_%o%o%o\n", CA[0],CA[1],CA[2],CA[3],CA[4],CA[5],CA[6],CA[7],CA[8],CA[9]);
return 0;
}

int G2INIT::DSPCODE(int8_t * CODE, uint8_t * CACODE) {
uint16_t idx;
  for (idx=0; idx<1023; idx++) {
    CODE[idx] = (CACODE[idx] == 1) ? 1 : -1;
  }
return 0;
}
