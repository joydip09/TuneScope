#pragma once

#include <Arduino.h>

namespace Config {

constexpr const char *APP_NAME = "TuneScope";
constexpr const char *APP_VERSION = "1.0.0";

// OLED
constexpr uint8_t OLED_WIDTH = 128;
constexpr uint8_t OLED_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;

// Audio
constexpr uint32_t SAMPLE_RATE = 16000;
constexpr uint8_t BITS_PER_SAMPLE = 16;
constexpr uint8_t CHANNELS = 1;

constexpr uint16_t RECORD_DURATION_SEC = 5;
constexpr uint32_t AUDIO_BUFFER_SIZE =
    SAMPLE_RATE * RECORD_DURATION_SEC * (BITS_PER_SAMPLE / 8) * CHANNELS;

// Recognition
constexpr uint32_t RECORD_DURATION_MS = 5000;
constexpr uint32_t RECORD_SAMPLE_RATE = 16000;
constexpr uint16_t RECORD_CHANNELS = 1;
constexpr uint16_t RECORD_BITS_PER_SAMPLE = 16;

constexpr bool ENABLE_MOCK_RECOGNITION = false;
constexpr char kRecordingPath[] = "/recording.wav";

// Wi-Fi
constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 15000;
constexpr uint32_t WIFI_RETRY_INTERVAL_MS = 5000;

// Display
constexpr uint16_t DISPLAY_REFRESH_MS = 33;

// Button
constexpr uint16_t DEBOUNCE_TIME_MS = 50;
constexpr uint16_t LONG_PRESS_TIME_MS = 2000;

// Debug
constexpr bool DEBUG_SERIAL = true;

} // namespace Config
