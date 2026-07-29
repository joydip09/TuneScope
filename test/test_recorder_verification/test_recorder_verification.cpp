#include <Arduino.h>
#include <unity.h>

#define TUNESCOPE_ENABLE_RECORDER_VERIFICATION 1

#include "audio.h"
#include "config.h"
#include "recorder.h"
#include "recorder_verification.h"
#include "wav.h"

// PlatformIO excludes src/ from embedded unit-test builds by default.
#include "../../src/audio.cpp"
#include "../../src/recorder.cpp"
#include "../../src/wav.cpp"
#include "../../src/recorder_verification.cpp"

namespace {

Recorder recorder;
WavGenerator wav;

bool recordAndBuildWav(RecorderValidationReport &report) {
  if (!recorder.startRecording() ||
      !wav.build(recorder.pcmData(), recorder.sampleCount(),
                 Config::RECORD_SAMPLE_RATE, Config::RECORD_CHANNELS,
                 Config::RECORD_BITS_PER_SAMPLE)) {
    return false;
  }

  const bool valid = RecorderVerification::validate(recorder, wav, report);
  RecorderVerification::printRecorderDiagnostics(report);
  RecorderVerification::printWavInfo(wav.data(), wav.sizeBytes());
  return valid;
}

void test_recording_to_wav_pipeline() {
  RecorderValidationReport report;
  TEST_ASSERT_TRUE(recordAndBuildWav(report));
  TEST_ASSERT_EQUAL_UINT32(80000, report.sampleCount);
  TEST_ASSERT_EQUAL_UINT32(160000, report.pcmSizeBytes);
  TEST_ASSERT_EQUAL_UINT32(160044, report.wavSizeBytes);
}

void test_multiple_recording_and_wav_cycles() {
  RecorderValidationReport firstReport;
  RecorderValidationReport secondReport;
  TEST_ASSERT_TRUE(recordAndBuildWav(firstReport));
  TEST_ASSERT_TRUE(recordAndBuildWav(secondReport));

  wav.clear();
  recorder.clear();
  TEST_ASSERT_NULL(wav.data());
  TEST_ASSERT_NULL(recorder.pcmData());
}

} // namespace

void setup() {
  Serial.begin(115200);
  delay(1000);
  UNITY_BEGIN();
  TEST_ASSERT_TRUE_MESSAGE(Audio::begin(), "Audio driver initialization failed");
  TEST_ASSERT_TRUE_MESSAGE(recorder.begin(), "PSRAM initialization failed");
  RUN_TEST(test_recording_to_wav_pipeline);
  RUN_TEST(test_multiple_recording_and_wav_cycles);
  UNITY_END();
}

void loop() {}
