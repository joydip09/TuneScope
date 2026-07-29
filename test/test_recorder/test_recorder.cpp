#include <Arduino.h>
#include <unity.h>

#include "audio.h"
#include "config.h"
#include "recorder.h"

// PlatformIO excludes src/ from embedded unit-test builds by default. Include
// only the two production units under test so this hardware test exercises the
// same recorder and audio-driver code that is built into the firmware.
#include "../../src/audio.cpp"
#include "../../src/recorder.cpp"

namespace {

Recorder recorder;
constexpr size_t kExpectedSamples =
    static_cast<size_t>(Config::RECORD_SAMPLE_RATE) *
    Config::RECORD_DURATION_SEC;
constexpr size_t kExpectedBytes = kExpectedSamples * sizeof(int16_t);

void test_psram_and_recorder_setup() {
  TEST_ASSERT_TRUE(psramFound());
  TEST_ASSERT_TRUE(recorder.begin());
}

void test_five_second_recording_and_cleanup() {
  const uint32_t startedAtMs = millis();
  TEST_ASSERT_TRUE(recorder.startRecording());
  const uint32_t elapsedMs = millis() - startedAtMs;

  TEST_ASSERT_TRUE(recorder.isFinished());
  TEST_ASSERT_NOT_NULL(recorder.pcmData());
  TEST_ASSERT_EQUAL_UINT32(kExpectedSamples, recorder.sampleCount());
  TEST_ASSERT_EQUAL_UINT32(kExpectedBytes, recorder.bufferSizeBytes());
  TEST_ASSERT_UINT32_WITHIN(500, Config::RECORD_DURATION_MS, elapsedMs);
  TEST_ASSERT_UINT32_WITHIN(20, elapsedMs, recorder.recordingDurationMs());

  // Accessing both boundary frames verifies that the complete declared PCM
  // range is present and readable. Zero is intentionally accepted: silence is
  // valid 16-bit PCM data.
  const int16_t firstSample = recorder.pcmData()[0];
  const int16_t lastSample = recorder.pcmData()[kExpectedSamples - 1];
  Serial.printf("Recorder test: first sample=%d, last sample=%d.\n",
                firstSample, lastSample);

  recorder.clear();
  TEST_ASSERT_NULL(recorder.pcmData());
  TEST_ASSERT_EQUAL_UINT32(0, recorder.sampleCount());
  TEST_ASSERT_EQUAL_UINT32(0, recorder.bufferSizeBytes());
}

void test_consecutive_recordings() {
  TEST_ASSERT_TRUE(recorder.startRecording());
  TEST_ASSERT_EQUAL_UINT32(kExpectedSamples, recorder.sampleCount());
  recorder.clear();

  TEST_ASSERT_TRUE(recorder.startRecording());
  TEST_ASSERT_EQUAL_UINT32(kExpectedSamples, recorder.sampleCount());
  recorder.clear();
}

} // namespace

void setup() {
  Serial.begin(115200);
  delay(1000);

  UNITY_BEGIN();
  TEST_ASSERT_TRUE_MESSAGE(Audio::begin(), "Audio driver initialization failed");
  RUN_TEST(test_psram_and_recorder_setup);
  RUN_TEST(test_five_second_recording_and_cleanup);
  RUN_TEST(test_consecutive_recordings);
  UNITY_END();
}

void loop() {}
