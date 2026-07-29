#include "button.h"

#include "config.h"
#include "pins.h"

#include <Arduino.h>

namespace {
bool currentState = false;
bool previousReading = false;
bool pressedEvent = false;

unsigned long lastDebounceTime = 0;
unsigned long pressStartTime = 0;
} // namespace

void Button::begin() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  previousReading = digitalRead(BUTTON_PIN);
  currentState = previousReading;
  pressedEvent = false;

  lastDebounceTime = 0;
  pressStartTime = 0;
}

void Button::update() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != previousReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > Config::DEBOUNCE_TIME_MS) {
    if (reading != currentState) {
      currentState = reading;

      if (currentState == LOW) {
        pressedEvent = true;
      }
    }
  }

  previousReading = reading;
}

bool Button::wasPressed() {
  if (pressedEvent) {
    pressedEvent = false;
    return true;
  }

  return false;
}

bool Button::isPressed() { return currentState == LOW; }