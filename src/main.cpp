#include <Arduino.h>

#include "button.h"

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(10);
  }

  Button::begin();

  Serial.println("Button test started.");
}

void loop() {
  Button::update();

  if (Button::wasPressed()) {
    Serial.println("Pressed!");
  }

  delay(5);
}