#include <Arduino.h>
#include <LittleFS.h>

#include "audio.h"
#include "button.h"
#include "config.h"
#include "recognizer.h"
#include "recorder.h"
#include "wifi_manager.h"

namespace {

Recorder recorder;
Recognizer recognizer;
bool recorderReady = false;
bool recognizerReady = false;
bool fileSystemReady = false;

void printSongInfo(const SongInfo &songInfo) {
  if (!songInfo.found) {
    Serial.println(F("[Main] Song Found : NO"));
  } else {
    Serial.println(F("[Main] Song Found : YES"));
    Serial.printf("[Main] Title  : %s\n", songInfo.title.c_str());
    Serial.printf("[Main] Artist : %s\n", songInfo.artist.c_str());
    Serial.printf("[Main] Album  : %s\n", songInfo.album.c_str());
    Serial.printf("[Main] Link   : %s\n", songInfo.songLink.c_str());
  }

  if (!songInfo.statusMessage.isEmpty()) {
    Serial.printf("[Main] Status : %s\n", songInfo.statusMessage.c_str());
  }
}

void runRecognitionCycle() {
  Serial.println(F("[Main] Recognizing..."));

  const SongInfo songInfo = recognizer.recognize();
  printSongInfo(songInfo);
  Serial.println(
      F("[Main] Recognition cycle complete. Press the button again."));
}

} // namespace

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(F("TuneScope Recognition Test"));
  Serial.println(F("[Main] Serial initialized."));

  if (!Audio::begin()) {
    Serial.println(F("Audio Driver initialization failed."));
  } else if (!recorder.begin()) {
    Serial.println(F("Recorder initialization failed: PSRAM unavailable."));
  } else {
    recorderReady = true;
    Serial.println(F("Recorder initialized; PSRAM detected."));
  }

  if (!recognizer.begin()) {
    Serial.println(F("Recognizer initialization failed."));
    recognizerReady = false;
  } else {
    recognizerReady = true;
    Serial.println(F("Recognizer initialized."));
  }

  Serial.println(F("[Main] Initializing Wi-Fi..."));
  WiFiManager::begin();

  Button::begin();

  fileSystemReady = LittleFS.begin(true);
  if (fileSystemReady) {
    Serial.println(F("[Main] LittleFS initialized. Waiting for button..."));
  } else {
    Serial.println(F("LittleFS initialization failed; continuing without it."));
  }
}

void loop() {
  Button::update();
  WiFiManager::update();

  const bool buttonPressed = Button::wasPressed();
  if (buttonPressed && !WiFiManager::isConnected()) {
    Serial.println(F("[Main] Wi-Fi is not connected; waiting for network."));
  } else if (buttonPressed && recorderReady && recognizerReady) {
    Serial.println(F("[Main] Button pressed."));
    runRecognitionCycle();
  }

  delay(10);
}
