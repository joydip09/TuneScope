#include <Arduino.h>

#include "wifi_manager.h"

void setup() {
  Serial.begin(115200);

  WiFiManager::begin();
}

void loop() {
  WiFiManager::update();

  switch (WiFiManager::state()) {

  case WiFiState::CONNECTING:
    break;

  case WiFiState::CONNECTED:
    break;

  case WiFiState::DISCONNECTED:
    break;
  }

  delay(10);
}