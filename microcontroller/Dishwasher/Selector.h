#ifndef Selector_h
#define Selector_h

#include <Arduino.h>

namespace Selector
{
  int latchPin = 12; // Pin connected to ST_CP of 74HC595（Pin12）
  int clockPin = 13; // Pin connected to SH_CP of 74HC595（Pin11）
  int dataPin = 11; // Pin connected to DS of 74HC595（Pin14）

  void setup()
  {
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);  
  }
  #define NUM_BUTTONS 6
  // ref: 10k
  // val: 4k7, 2k2, 1k, 470, 20, 100
  // cal: ref over ref+val * 1024 (ADC) 
  int buttonMap[NUM_BUTTONS] = {695, 838, 930, 977, 1002, 1014};

  int selectPin = A0;

  int select_button() {
    int raw = analogRead(selectPin);
    // Serial.print("Button: "); Serial.println(raw);
    int buttonPressed = 0;
    // map to button code
    for (int i = 0; i < NUM_BUTTONS; i++) {
      int ref = buttonMap[i];
      int delta = raw-ref;
      if (delta < 0) delta = -delta;
      if (delta < 5) {
        buttonPressed = i+1;
        break;
      }
    }
    return buttonPressed;  
  }

  void show_ledbar(byte x) {
      // Output low level to latchPin
      digitalWrite(latchPin, LOW);
      // Send serial data to 74HC595
      shiftOut(dataPin, clockPin, LSBFIRST, x);
      // Output high level to latchPin, and 74HC595 will update the data to the parallel output port.
      digitalWrite(latchPin, HIGH);  
  }

  void off_buttons() {
    byte x_off = 0x00;
    show_ledbar(x_off);
  }

  int selected_button = 3;
  
  void flash_on(int button)
  {
    int i = button-1;
    byte x_on = 0x01 << i;
    show_ledbar(x_on);    
  }

  void flash_off()
  {
    off_buttons();
  }
  
  int select() {
    // get the selected button
    int button = select_button();
    // flash selected button
    if (button != 0)
      selected_button = button;
    return selected_button;
  }
}

#endif
