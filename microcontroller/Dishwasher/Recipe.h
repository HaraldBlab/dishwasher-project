#ifndef Recipe_h
#define Recipe_h

#include <Arduino.h>

namespace Recipe {
  byte pgm1[] = {2, 1, 5};
  byte pgm2[] = {2, 4, 0};
  byte pgm3[] = {3, 4, 0};
  byte pgm4[] = {4, 0, 0};
  byte pgm5[] = {1, 3, 0};
  byte pgm6[] = {1, 0, 0};

  byte pgm[3];

  int state = 0;

  void copy(byte bit[3])
  {
    pgm[0] = bit[0];
    pgm[1] = bit[1];
    pgm[2] = bit[2];
  }

  void select(int next, byte bit[3]) {
    if (state != next)
      copy(bit);
    state = next;   
  }

  void work(byte bit[3])
  {
    int dec = 1;
    if (dec == 1) {
      if (bit[2] == 0) {
        bit[2] = 9;
      } else {
        bit[2]--;
        dec=0;
      }
    }
    if (dec == 1) {
      if (bit[1] == 0) {
        bit[1] = 5;
      } else {
        bit[1]--;
        dec=0;
      }
    }
    if (dec == 1) {
      if (bit[0] > 0) {
        bit[0]--;
        dec=0;
      }
    }
  }

  void select(int next) {
     if (next == 6) {
        select(6, pgm6);
     } else if (next == 5) { 
        select(5, pgm5);
     } else if (next == 4) {
        select(4, pgm4);
     } else if (next == 3) { 
        select(3, pgm3);
     } else if (next == 2) { 
        select(2, pgm2);
     } else if (next == 1) {
        select(1, pgm1);
     }  
  }
}

#endif
