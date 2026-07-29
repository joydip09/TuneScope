#include <Arduino.h>
#include <LittleFS.h>

#include "audio.h"
#include "button.h"
#include "config.h"
#include "recognizer.h"
#include "recorder.h"
#include "recorder_verification.h"
#include "secrets.h"
#include "wav.h"
#include "wifi_manager.h"

namespace {

Recorder recorder;
WavGenerator wav;
Recognizer recognizer;
bool recorderReady = false;
bool fileSystemReady = false;

constexpr char kAuddEndpoint[] = "https://api.audd.io/";

bool saveWavFile(const WavGenerator &wavFile) {
  if (!fileSystemReady) {
    Serial.println(F("File save failed: LittleFS is unavailable."));
    return false;
  }

  if (LittleFS.exists(Config::kRecordingPath) &&
      !LittleFS.remove(Config::kRecordingPath)) {
    Serial.println(F("File save failed: could not replace recording.wav."));
    return false;
  }

  File output = LittleFS.open(Config::kRecordingPath, FILE_WRITE);
  if (!output) {
    Serial.println(F("File save failed: could not open recording.wav."));
    return false;
  }

  const size_t bytesWritten = output.write(wavFile.data(), wavFile.sizeBytes());
  output.close();

  if (bytesWritten != wavFile.sizeBytes()) {
    Serial.printf("File save failed: wrote %u of %u bytes.\n",
                  static_cast<unsigned>(bytesWritten),
                  static_cast<unsigned>(wavFile.sizeBytes()));
    return false;
  }

  Serial.printf("File saved: %s (%u bytes).\n", Config::kRecordingPath,
                static_cast<unsigned>(bytesWritten));
  return true;
}

void printValidationResult(const RecorderValidationReport &report,
                           bool pipelineValid) {
  Serial.printf("Recording duration: %u ms\n",
                static_cast<unsigned>(report.recordingDurationMs));
  Serial.printf("Sample count: %u\n",
                static_cast<unsigned>(report.sampleCount));
  Serial.printf("PCM size: %u bytes\n",
                static_cast<unsigned>(report.pcmSizeBytes));
  Serial.printf("WAV size: %u bytes\n",
                static_cast<unsigned>(report.wavSizeBytes));
  Serial.printf("WAV format: %u Hz, %u channel(s), %u-bit PCM\n",
                static_cast<unsigned>(Config::RECORD_SAMPLE_RATE),
                static_cast<unsigned>(Config::RECORD_CHANNELS),
                static_cast<unsigned>(Config::RECORD_BITS_PER_SAMPLE));
  Serial.printf("WAV header: %s; PCM placement: %s; pipeline: %s\n",
                report.wavHeaderValid ? "valid" : "INVALID",
                report.pcmPlacementValid ? "valid" : "INVALID",
                pipelineValid ? "PASS" : "FAIL");
}

void printSongInfo(const SongInfo &songInfo) {
  if (!songInfo.found) {
    Serial.println(F("[Main] Song Found : NO"));
    Serial.println(F("[Main] No matching song detected."));
    return;
  }

  Serial.println(F("[Main] Song Found : YES"));
  Serial.printf("[Main] Title  : %s\n", songInfo.title.c_str());
  Serial.printf("[Main] Artist : %s\n", songInfo.artist.c_str());
  Serial.printf("[Main] Album  : %s\n", songInfo.album.c_str());
  Serial.printf("[Main] Link   : %s\n", songInfo.songLink.c_str());
}

void runRecordingCycle() {
  Serial.println(F("[Main] Starting 5-second recording cycle..."));

  if (!recorder.startRecording()) {
    Serial.println(F("[Main] Recording failed."));
    return;
  }

  Serial.println(F("[Main] Recording completed."));

  if (!wav.build(recorder.pcmData(), recorder.sampleCount(),
                 Config::RECORD_SAMPLE_RATE, Config::RECORD_CHANNELS,
                 Config::RECORD_BITS_PER_SAMPLE)) {
    Serial.println(F("[Main] WAV generation failed."));
    return;
  }

  Serial.printf("[Main] WAV buffer ready: %u bytes\n",
                static_cast<unsigned>(wav.sizeBytes()));

  RecorderValidationReport report;
  const bool pipelineValid =
      RecorderVerification::validate(recorder, wav, report);
  printValidationResult(report, pipelineValid);

  if (!saveWavFile(wav)) {
    Serial.println(F("[Main] recording.wav was not saved."));
  }

  HttpsUploadRequest uploadRequest;
  uploadRequest.endpoint = kAuddEndpoint;
  uploadRequest.apiToken = AUDD_API_TOKEN;

  Serial.println(F("[Main] Uploading WAV over HTTPS..."));
  const HttpsUploadResult uploadResult =
      recognizer.uploadWav(wav.data(), wav.sizeBytes(), uploadRequest);
  Serial.printf("[Main] HTTPS upload finished. HTTP status: %d (%s)\n",
                uploadResult.httpStatusCode,
                uploadResult.success ? "success" : "failed");
  if (!uploadResult.errorMessage.isEmpty()) {
    Serial.printf("[Main] HTTPS upload detail: %s\n",
                  uploadResult.errorMessage.c_str());
  }

  if (uploadResult.httpStatusCode >= 0) {
    Serial.println(F("[Main] JSON response received."));
    printSongInfo(uploadResult.songInfo);
  }

  Serial.println(F("[Main] Cycle complete. Press the button again to repeat."));

  // Keep both buffers valid until the next button press so they can be
  // inspected through the debugger if a validation failure occurs.
}

} // namespace

void listFiles() {
  File root = LittleFS.open("/");

  File file = root.openNextFile();

  while (file) {
    Serial.printf("%s (%u bytes)\n", file.name(), (unsigned)file.size());

    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(F("TuneScope recorder test application"));
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
    recorderReady = false;
  } else {
    Serial.println(F("Recognizer initialized."));
  }

  Serial.println(F("[Main] Initializing Wi-Fi..."));
  WiFiManager::begin();

  Button::begin();

  // This temporary recorder test owns its debug filesystem output. Formatting
  // on an initial mount failure lets a freshly flashed device save its first
  // recording without any separate filesystem-upload step.
  fileSystemReady = LittleFS.begin(true);
  if (fileSystemReady) {
    Serial.println(F(
        "[Main] LittleFS initialized. Press the button to record and upload."));
  } else {
    Serial.println(
        F("LittleFS initialization failed; recordings cannot be saved."));
  }
}

void loop() {
  Button::update();
  WiFiManager::update();

  const bool buttonPressed = Button::wasPressed();
  if (buttonPressed && !WiFiManager::isConnected()) {
    Serial.println(F("[Main] Wi-Fi is not connected; waiting for network."));
  } else if (buttonPressed && recorderReady) {
    Serial.println(F("[Main] Button pressed. Starting test sequence..."));
    runRecordingCycle();

    listFiles();
  }

  delay(10);
}
