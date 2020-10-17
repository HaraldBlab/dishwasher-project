#ifndef OnOffButton_h
#define OnOffButton_h

#include <Arduino.h>

namespace OnOffButton
{
  int onOffPin = 4;

  void setup() {
    pinMode(onOffPin, INPUT_PULLUP);  
  }
  int pressed() {
      if (digitalRead(onOffPin) == LOW) {
        while (digitalRead(onOffPin) == LOW);
        return 1;
      }
      return 0;
  }
}

#endif
