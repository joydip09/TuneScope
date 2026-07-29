#include <Arduino.h>

#include "audio.h"
#include "config.h"

void setup() {
  if (Config::DEBUG_SERIAL) {
    Serial.begin(115200);

    while (!Serial) {
      delay(10);
    }

    Serial.println();
    Serial.println("=== TuneScope Audio Test ===");
  }

  if (!Audio::begin()) {
    Serial.println("Audio initialization failed.");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("Audio initialized.");
}

void loop() {
  Audio::update();

  delay(50);
}