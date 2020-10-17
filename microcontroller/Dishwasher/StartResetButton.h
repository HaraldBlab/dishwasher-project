#ifndef StartResetButton_h
#define StartResetButton_h

#include <Arduino.h>

namespace StartResetButton
{
  int startPin = 5;

  void setup() {
    pinMode(startPin, INPUT_PULLUP);  
  }
  int pressed() {
      if (digitalRead(startPin) == LOW) {
        while (digitalRead(startPin) == LOW);
        return 1;
      }
      return 0;
  }
}

#endif
