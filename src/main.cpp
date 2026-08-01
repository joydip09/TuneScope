#include <Arduino.h>
#include <LittleFS.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "audio.h"
#include "button.h"
#include "config.h"
#include "display_manager.h"
#include "pins.h"
#include "recognizer.h"
#include "recorder.h"
#include "wifi_manager.h"

namespace {

bool lastModeButtonPressedState = false;
bool debouncedModeButtonPressedState = false;
unsigned long lastModeButtonChangeMs = 0;

Recorder recorder;
Recognizer recognizer;
bool recorderReady = false;
bool recognizerReady = false;
bool fileSystemReady = false;
DisplayManager displayManager;
TaskHandle_t recognitionTaskHandle = nullptr;

struct RecognitionTaskContext {
  Recognizer *recognizer;
};

void recognitionTaskEntry(void *parameter) {
  auto *context = static_cast<RecognitionTaskContext *>(parameter);
  if (context != nullptr && context->recognizer != nullptr) {
    context->recognizer->recognize();
  }

  recognitionTaskHandle = nullptr;
  vTaskDelete(nullptr);
}

void updateDisplayModeButton() {
  const bool rawButtonPressed = (digitalRead(MODE_BUTTON_PIN) == LOW);
  const unsigned long now = millis();

  if (rawButtonPressed != lastModeButtonPressedState) {
    lastModeButtonChangeMs = now;
    lastModeButtonPressedState = rawButtonPressed;
  }

  if ((now - lastModeButtonChangeMs) < Config::DEBOUNCE_TIME_MS) {
    return;
  }

  if (rawButtonPressed == debouncedModeButtonPressedState) {
    return;
  }

  debouncedModeButtonPressedState = rawButtonPressed;

  if (rawButtonPressed) {
    displayManager.nextDisplayMode();
  }
}

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

void startRecognitionCycle() {
  if (recognitionTaskHandle != nullptr) {
    return;
  }

  Serial.println(F("[Main] Recognizing..."));
  displayManager.setDisplayMode(DisplayMode::SongDetails);
  displayManager.setRecognitionState(RecognitionState::Recording);

  RecognitionTaskContext context{&recognizer};
  xTaskCreate(recognitionTaskEntry, "recognition_task", 8192, &context, 1,
              &recognitionTaskHandle);
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

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  Button::begin();
  recognizer.setObserver(&displayManager);
  displayManager.setRecognitionState(RecognitionState::Idle);

  fileSystemReady = LittleFS.begin(true);
  if (fileSystemReady) {
    Serial.println(F("[Main] LittleFS initialized. Waiting for button..."));
  } else {
    Serial.println(F("LittleFS initialization failed; continuing without it."));
  }
}

void loop() {
  Button::update();
  updateDisplayModeButton();
  WiFiManager::update();
  const bool buttonPressed = Button::wasPressed();
  if (buttonPressed && !WiFiManager::isConnected()) {
    Serial.println(F("[Main] Wi-Fi is not connected; waiting for network."));
  } else if (buttonPressed && recorderReady && recognizerReady) {
    Serial.println(F("[Main] Button pressed."));
    startRecognitionCycle();
  }

  // Update display after handling input to avoid blocking audio reads
  // (Audio::update() can block inside DisplayManager::update()).
  displayManager.update();

  delay(10);
}
