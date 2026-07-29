#pragma once

#include "wifi_manager.h"

#include <Arduino.h>

class Display {
public:
  static bool begin();

  static void clear();
  static void update();

  static void showSplash();
  static void showWiFiStatus(WiFiState state, IPAddress ip);
  static void showAudioLevel(uint16_t rms);
};