#include <Arduino.h>

#include "display.h"

void setup() {
  if (!Display::begin()) {
    while (true) {
      delay(100);
    }
  }

  Display::showSplash();
}

void loop() { Display::update(); }