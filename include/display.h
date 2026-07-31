#pragma once

#include "types.h"
#include "wifi_manager.h"

#include <Arduino.h>

class Display {
public:
  static bool begin();

  static void clear();
  static void update();

  static void showSplash();
  static bool showSongDetails(RecognitionState state, const SongInfo &songInfo,
                              int16_t &scrollOffset,
                              int16_t &maxScrollDistance);
  static void showVisualizer(uint16_t rms);
  static void showWiFiStatus(WiFiState state, IPAddress ip);
  static void showAudioLevel(uint16_t rms);
};