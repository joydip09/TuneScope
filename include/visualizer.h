#pragma once

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class Visualizer {
public:
  static void draw(Adafruit_SSD1306 &display, float normalizedAmplitude);

private:
  static constexpr int16_t kDisplayWidth = 128;
  static constexpr int16_t kDisplayHeight = 64;
  static constexpr int16_t kCenterX = kDisplayWidth / 2;
  static constexpr int16_t kCenterY = kDisplayHeight / 2;
  static constexpr int16_t kBarsPerSide = 8;
  static constexpr int16_t kBarWidth = 3;
  static constexpr int16_t kBarGap = 1;
  static constexpr int16_t kBarMinHeight = 4;
  static constexpr int16_t kBarMaxHeight = 44;
  static constexpr float kSilenceThreshold = 0.05f;
  static constexpr float kSmoothingFactor = 0.18f;
  static const float kProfileMultiplier[kBarsPerSide];

  static float smoothedAmplitude_;

  static void drawBaselineDots(Adafruit_SSD1306 &display);
  static void drawWaveform(Adafruit_SSD1306 &display, float amplitude);
};
