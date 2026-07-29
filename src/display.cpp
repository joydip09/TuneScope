#include "display.h"

#include "config.h"
#include "pins.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace {

Adafruit_SSD1306 display(Config::OLED_WIDTH, Config::OLED_HEIGHT, &Wire, -1);

void drawHorizontalBar(int x, int y, int width, int height, uint8_t percent) {
  percent = constrain(percent, 0, 100);

  display.drawRect(x, y, width, height, SSD1306_WHITE);

  int fillWidth = ((width - 2) * percent) / 100;

  display.fillRect(x + 1, y + 1, fillWidth, height - 2, SSD1306_WHITE);
}

} // namespace

bool Display::begin() {
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS))
    return false;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();

  return true;
}

void Display::clear() { display.clearDisplay(); }

void Display::update() { display.display(); }

void Display::showSplash() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("TuneScope");

  display.setTextSize(1);
  display.println();
  display.println("Ready");

  display.display();
}

void Display::showWiFiStatus(WiFiState state, IPAddress ip) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println("Wi-Fi");
  display.println();

  switch (state) {
  case WiFiState::CONNECTING:
    display.println("Connecting...");
    break;

  case WiFiState::CONNECTED:
    display.println("Connected");
    display.println(ip);
    break;

  case WiFiState::DISCONNECTED:
    display.println("Disconnected");
    break;
  }

  display.display();
}

void Display::showAudioLevel(uint16_t rms) {
  display.clearDisplay();

  Serial.print("RMS: ");
  Serial.println(rms);

  long level = map(rms, 500, 5000, 0, 100);
  level = constrain(level, 0L, 100L);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Audio Level");

  drawHorizontalBar(8, 20, 112, 12, static_cast<uint8_t>(level));

  display.setCursor(0, 42);
  display.print("RMS: ");
  display.println(rms);

  display.display();
}