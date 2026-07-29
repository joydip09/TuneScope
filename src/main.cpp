#include <Arduino.h>

#include "audio.h"
#include "button.h"
#include "display.h"
#include "wifi_manager.h"

enum class Screen { SPLASH, WIFI, AUDIO };

Screen currentScreen = Screen::SPLASH;

void updateScreen() {
  switch (currentScreen) {
  case Screen::SPLASH:
    Display::showSplash();
    break;

  case Screen::WIFI:
    Display::showWiFiStatus(WiFiManager::state(), WiFiManager::localIP());
    break;

  case Screen::AUDIO:
    Display::showAudioLevel(Audio::getRMS());
    break;
  }
}

void setup() {
  Serial.begin(115200);

  Display::begin();
  Audio::begin();
  Button::begin();
  WiFiManager::begin();

  updateScreen();
}

void loop() {
  Button::update();
  Audio::update();
  WiFiManager::update();

  if (Button::wasPressed()) {

    switch (currentScreen) {
    case Screen::SPLASH:
      currentScreen = Screen::WIFI;
      break;

    case Screen::WIFI:
      currentScreen = Screen::AUDIO;
      break;

    case Screen::AUDIO:
      currentScreen = Screen::SPLASH;
      break;
    }

    updateScreen();
  }

  if (currentScreen == Screen::WIFI) {
    Display::showWiFiStatus(WiFiManager::state(), WiFiManager::localIP());
  } else if (currentScreen == Screen::AUDIO) {
    Display::showAudioLevel(Audio::getRMS());
  }

  delay(10);
}