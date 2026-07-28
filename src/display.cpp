#include "display.h"

#include "config.h"
#include "pins.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace {

Adafruit_SSD1306 display(Config::OLED_WIDTH, Config::OLED_HEIGHT, &Wire, -1);

}

bool Display::begin() {
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS)) {
    return false;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  return true;
}

void Display::clear() {
  display.clearDisplay();
  display.display();
}

void Display::showSplash() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(8, 8);
  display.println(Config::APP_NAME);

  display.setTextSize(1);
  display.setCursor(28, 36);
  display.print("Version ");
  display.println(Config::APP_VERSION);

  display.setCursor(18, 52);
  display.print("Initializing...");

  display.display();
}

void Display::showMessage(const char *message) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(message);

  display.display();
}

void Display::update() {}