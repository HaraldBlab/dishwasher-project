#ifndef ClockDisplay_h
#define ClockDisplay_h

#include <TM1637.h>

namespace ClockDisplay {

  // Instantiation and pins configurations
  // Pin 3 - > DIO
  // Pin 2 - > CLK
  TM1637 tm(2, 3);

  int second;

  void off()
  {
    tm.colonOff();
    tm.clearScreen();
  }

  void setup() {
    int second = 0;
    tm.begin();
    tm.setBrightness(4);
    off();
  }

  void flash_on(byte bit[3]) {
    int val = bit[0]*100 + bit[1]*10 + bit[2];
    tm.colonOn();
    tm.display(val, false, false, 0);
  }
  void flash_off() {
    off();
  }
}
#endif
