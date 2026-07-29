#pragma once

#include <Arduino.h>

enum class WiFiState { DISCONNECTED, CONNECTING, CONNECTED };

class WiFiManager {
public:
  static bool begin();
  static void update();

  static bool isConnected();
  static IPAddress localIP();
  static WiFiState state();
};