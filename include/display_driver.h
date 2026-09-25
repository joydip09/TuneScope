#pragma once

#include "config.h"

#include <Wire.h>

#if TUNESCOPE_DISPLAY_SH1106
#include <Adafruit_SH110X.h>

class DisplayDriver : public Adafruit_SH1106G {
public:
  DisplayDriver(uint16_t width, uint16_t height, TwoWire *wire,
                int8_t resetPin)
      : Adafruit_SH1106G(width, height, wire, resetPin) {}

  bool begin(uint8_t address) {
    return Adafruit_SH1106G::begin(address, true);
  }
};

constexpr uint16_t DISPLAY_WHITE = SH110X_WHITE;
constexpr uint16_t DISPLAY_BLACK = SH110X_BLACK;
#else
#include <Adafruit_SSD1306.h>

class DisplayDriver : public Adafruit_SSD1306 {
public:
  DisplayDriver(uint16_t width, uint16_t height, TwoWire *wire,
                int8_t resetPin)
      : Adafruit_SSD1306(width, height, wire, resetPin) {}

  bool begin(uint8_t address) {
    return Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC, address);
  }
};

constexpr uint16_t DISPLAY_WHITE = SSD1306_WHITE;
constexpr uint16_t DISPLAY_BLACK = SSD1306_BLACK;
#endif