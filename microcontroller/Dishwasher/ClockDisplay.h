#ifndef ClockDisplay_h
#define ClockDisplay_h

#include <Arduino.h>
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
    tm.display("    ", false, false, 0);
  }

  void setup() {
    int second = 0;
    tm.begin();
    tm.setBrightness(4);
    off();
  }

  void flash_on(byte bit[3], bool leading=false) {
    int val = bit[0]*100 + bit[1]*10 + bit[2];
    String val_string(val);
    String to_display("    ");
    if (leading) {
      if (val_string.length() == 3)
        to_display.replace("    ", " ");
      if (val_string.length() == 2)
        to_display.replace("    ", " 0");
      if (val_string.length() == 1)
        to_display.replace("    ", " 00");
    } else
      to_display.remove(0, val_string.length());
    to_display.concat(val_string);
    tm.colonOn();
    tm.display(to_display, false, false, 0);
  }
  void flash_off() {
    off();
  }
}
#endif
