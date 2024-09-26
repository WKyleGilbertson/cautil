// Filename : G2DELAY.cpp

using namespace std;
#include <cstdio>
#include "G2DELAY.h"

#define TAP1    0x0200
#define TAP2    0x0100
#define TAP3    0x0080
#define TAP6    0x0010
#define TAP8    0x0004
#define TAP9    0x0002
#define TAP10   0x0001

G2DELAY::G2DELAY(int PRN, int CODEPHASE) {
  cphase = CODEPHASE % 1023;
  prn = PRN;
  PRNGEN(CACODE, prn, cphase);
  DSPCODE(CODE, CACODE);
}

int G2DELAY::PRNGEN(char * CACODE, int PRN, int CODEPHASE) {
//int G2DELAY::PRNGEN(G2DELAY * SV, int PRN, int CODEPHASE) {
  /* Set G1 and G2 to initial values of 0x3FF */
  /* Set G2 Delay to desired value to get pattern */
  /* Alternatively, set Delay to zero, and G1 to 0x3FF, but set G2 to a value */
  /* First method is Figure A-1 in DO-229E. Second method is Figure A-2 */
  /* Table A-1 contains PRN number and G2 delay / Initial G2 and 1st 10 chips */
  /* IS-GPS-200 only provides delay, not G2, so that has to be generated */
  /* Or dealy has to be used everywhere. Initial G2 seems more streamlined */
  unsigned short GPSDelays[37] = {5, 6, 7, 8, 17, 18, 139, 140, 141, 251, 252,
  254, 255, 256, 257, 258, 469, 470, 471, 472, 473, 474, 509, 512, 513, 514,
  515, 516, 859, 860, 861, 862, 863, 950, 947, 948, 950};
  unsigned short SBASDelays[39] = {145, 175, 52, 21, 237, 235, 886, 657, 634,
    762, 355, 1012, 176, 603, 130, 359, 595, 68, 386, 797, 456, 499, 883, 307,
    127, 211, 121, 118, 163, 628, 853, 484, 289, 811, 202, 1021, 463, 568, 904};
  unsigned short G1=0x03FF;
  unsigned short G2=0x03FF;
//  unsigned short G2=0x0258; // Octal Inverse of o1626, or first ten chips? PRN9
  signed char G1t[1023], G2t[1023], CA[1023];
  unsigned short CAbit = 0, G1FeedBackBit = 0x0000, G2FeedBackBit = 0x0000;
  unsigned short Delay = 0, F10Chips = 0x0000, F10Inverse = 0x0000;
  short int idx = 0, cnt=0;
  Delay = (PRN < 100) ? GPSDelays[PRN-1] : SBASDelays[PRN-120]; 
   for (idx=0; idx<2048; idx++) {
    G1a[cnt%1023] = G1;
//    if (idx == Delay) printf("\nG1:0x%.4X \n", G1);
     G1t[cnt%1023] = (G1 & TAP10) ? 1 : 0;
//     printf("%d", G1t); // Meaningless until delay has expired
//     printf("%d", G1t[cnt%1023]); // Meaningless until delay has expired
     G1FeedBackBit = ((G1 & TAP3) >> 7) ^ (G1 & TAP10);
     G1 |= (G1FeedBackBit << 10);
     G1 = G1 >> 1;
//    printf("%.3X ", G1); /* Prove G1 */
     if (idx >= Delay) { // Delay of 141 for PRN 9?
     G2a[cnt%1023] = G2;
     G2t[cnt%1023] = (G2 & TAP10) ? 1 : 0;
     G2FeedBackBit =
      ((G2 & TAP2) >> 8) ^ ((G2 & TAP3) >> 7) ^ ((G2 & TAP6) >> 4) ^
      ((G2 & TAP8) >> 2) ^ ((G2 & TAP9) >> 1) ^ (G2 & TAP10);
     G2 |= (G2FeedBackBit << 10);
     G2 = G2 >> 1;
//    printf("%.3X ", G2); /* Prove G2 */
     }

     CA[cnt%1023] = G1t[cnt%1023] ^ G2t[cnt%1023];
     cnt++;
/*     if (G1 == 0x3FF) { // Used for Epoch detection for G1 and G2
       printf(" %d G2:0x%.4X \n", cnt, G2);
       cnt = 0;
     } */
   }
  F10Chips = (CA[0]<<9) | (CA[1]<<8) | (CA[2]<<7) | (CA[3]<<6) | (CA[4]<<5) |
           (CA[5]<<4) | (CA[6]<<3) | (CA[7]<<2) | (CA[8]<<1) | CA[9];
  F10 = F10Chips;
  F10Inverse = (CA[0]<<0) | (CA[1]<<1) | (CA[2]<<2) | (CA[3]<<3) | (CA[4]<<4) |
           (CA[5]<<5) | (CA[6]<<6) | (CA[7]<<7) | (CA[8]<<8) | (CA[9]<<9);
  F10I = F10Inverse;
//  printf("o%.4o o%.4o\n", F10Chips, F10Inverse); // Verify First 10 Chips
//printf("%o%o%o%o%o%o%o%o%o%o\n", // Verify each of first 10 chips individualy
//    CA[0],CA[1],CA[2],CA[3],CA[4],CA[5], CA[6],CA[7],CA[8],CA[9]);

  for (idx=0; idx<1023; idx++) {
    CACODE[idx] = CA[(idx+CODEPHASE) % 1023];
//  printf("%.1X", CACODE[idx]);
  }
  return 0;
}

int G2DELAY::DSPCODE(char * CODE, char * CACODE) {
  int idx = 0;
  for (idx=0; idx<1023; idx++) {
    CODE[idx] = (CACODE[idx] == 1) ? 1 : -1;
  }
return 0;
}
