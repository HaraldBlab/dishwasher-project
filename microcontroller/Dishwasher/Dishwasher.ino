#include "OnOffButton.h"
#include "Selector.h"
#include "ClockDisplay.h"
#include "Recipe.h"
#include "StartResetButton.h"

#define STATE_INIT    0
#define STATE_SELECT  1
#define STATE_ONOFF   2
#define STATE_START   3
#define STATE_WASH    4
#define STATE_DONE    5

int state;

void setup() {
  Serial.begin(115200);
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

const unsigned long flashTime_on = 500UL;
const unsigned long flashTime_off = 1000UL;
unsigned long flashTime = 0UL;
bool flashing = false;
bool flashOff = true;

// loop handling pgm selection
void loop() {
  if (state == STATE_INIT) {
    Selector::off_buttons();
    Recipe::select(2);
    ClockDisplay::off();
    flashing = false;
    state = STATE_ONOFF;
  } else if (state == STATE_ONOFF) {
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
      unsigned long currentTime = millis();
      int button = Selector::select();
      Recipe::select(button);
      if (! flashing) {
        flashing = true;
        flashTime = currentTime;
        flashOff = true;
      } else if (currentTime - flashTime < flashTime_on){
        if (flashOff){
          Selector::flash_on(button);
          ClockDisplay::flash_on(Recipe::pgm);
          flashOff = false;
        }
      } else if (currentTime - flashTime < flashTime_off){
        if (! flashOff) {
          Selector::flash_off();
          ClockDisplay::flash_off();
          flashOff=true;
        }
      } else {
        flashing = false;
      }
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
        ClockDisplay::flash_on(Recipe::pgm, true);
        int val = Recipe::pgm[0]*100 + Recipe::pgm[1]*10 + Recipe::pgm[2];
        if (val < 1) {
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
