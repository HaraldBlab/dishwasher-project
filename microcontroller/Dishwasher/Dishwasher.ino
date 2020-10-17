#include "OnOffButton.h"
#include "Selector.h"
#include "ClockDisplay.h"
#include "Recipe.h"
#include "StartResetButton.h"

#define STATE_INIT    0
#define STATE_SELECT  1
#define STATE_START   2
#define STATE_WASH    3
#define STATE_DONE    4

int state;

void setup() {
  Serial.begin(9600);
  // set the on/off pin 
  OnOffButton::setup();
  // set the selector pins and led
  Selector::setup();
  // setup the cock display
  ClockDisplay::setup();
  // select the 1st recipe
  Recipe::select(2);
  // set the start pin
  StartResetButton::setup();
  // set the state
  state = STATE_INIT;
}

int on_button = 0; // already started?

// loop handling pgm selection
void loop() {
  if (state == STATE_INIT) {
    Selector::off_buttons();
    if (OnOffButton::pressed()) {
      on_button = 1;
      state = STATE_SELECT;
    }
    delay(200);
  } else if (state == STATE_SELECT) {
    if (OnOffButton::pressed()) {
      on_button = 0;
      state = STATE_INIT;
    } else if (StartResetButton::pressed()) {
      state = STATE_START;
    } else {
      int button = Selector::select();
      Recipe::select(button);
      Selector::flash_on(button);
      ClockDisplay::flash_on(Recipe::pgm);
      delay(500);
      Selector::flash_off();
      ClockDisplay::flash_off();
      delay(500);
    }
  } else if (state == STATE_START) {
      // lite the leds and the display
      int button = Selector::select();
      Selector::flash_on(button);
      ClockDisplay::flash_on(Recipe::pgm);
      state = STATE_WASH;
  } else if (state == STATE_WASH) {
      if (StartResetButton::pressed()) {
        state = STATE_DONE;
      } else {
        Recipe::work(Recipe::pgm);
        // TODO: need this to clear the 1 after 1:00 - 0:59
        // TODO: also clears the 1 after :10 - :09
        ClockDisplay::flash_off();
        ClockDisplay::flash_on(Recipe::pgm);
        int val = Recipe::pgm[0]*100 + Recipe::pgm[1]*10 + Recipe::pgm[2];
        if (val <= 1) {
          state = STATE_DONE;
        }
        delay(200);
      }
  } else if (state == STATE_DONE) {
        on_button = 0;
        Selector::flash_off();
        ClockDisplay::flash_off();
        state = STATE_INIT;
  }
}
