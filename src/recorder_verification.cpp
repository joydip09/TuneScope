#include "recorder_verification.h"

#include "config.h"
#include "recorder.h"
#include "wav.h"

#include <Arduino.h>
#include <cstring>

#ifndef TUNESCOPE_ENABLE_RECORDER_VERIFICATION
#define TUNESCOPE_ENABLE_RECORDER_VERIFICATION 0
#endif

namespace {

constexpr size_t kWavHeaderSize = WavGenerator::kHeaderSizeBytes;
constexpr size_t kExpectedSampleCount =
    static_cast<size_t>(Config::RECORD_SAMPLE_RATE) *
    Config::RECORD_DURATION_SEC;
constexpr size_t kExpectedPcmSizeBytes =
    kExpectedSampleCount * (Config::RECORD_BITS_PER_SAMPLE / 8);
constexpr uint32_t kDurationToleranceMs = 500;

uint16_t readUint16LE(const uint8_t *source) {
  return static_cast<uint16_t>(source[0]) |
         (static_cast<uint16_t>(source[1]) << 8);
}

uint32_t readUint32LE(const uint8_t *source) {
  return static_cast<uint32_t>(source[0]) |
         (static_cast<uint32_t>(source[1]) << 8) |
         (static_cast<uint32_t>(source[2]) << 16) |
         (static_cast<uint32_t>(source[3]) << 24);
}

bool durationIsValid(uint32_t durationMs) {
  const uint32_t expectedDurationMs = Config::RECORD_DURATION_MS;
  return durationMs >= expectedDurationMs - kDurationToleranceMs &&
         durationMs <= expectedDurationMs + kDurationToleranceMs;
}

} // namespace

bool RecorderVerification::validate(const Recorder &recorder,
                                    const WavGenerator &wav,
                                    RecorderValidationReport &report) {
  report = {};
  report.recordingDurationMs = recorder.recordingDurationMs();
  report.sampleCount = recorder.sampleCount();
  report.pcmSizeBytes = recorder.bufferSizeBytes();
  report.wavSizeBytes = wav.sizeBytes();

  report.recordingFinished = recorder.isFinished();
  report.durationValid = durationIsValid(report.recordingDurationMs);
  report.sampleCountValid = report.sampleCount == kExpectedSampleCount;
  report.pcmSizeValid = report.pcmSizeBytes == kExpectedPcmSizeBytes;
  report.wavHeaderValid = validateWavHeader(wav.data(), wav.sizeBytes());
  report.wavSizeValid =
      report.wavSizeBytes == kWavHeaderSize + report.pcmSizeBytes &&
      wav.pcmSizeBytes() == report.pcmSizeBytes;

  report.pcmPlacementValid = report.wavSizeValid && recorder.pcmData() != nullptr &&
                             wav.data() != nullptr &&
                             std::memcmp(wav.data() + kWavHeaderSize,
                                         recorder.pcmData(), report.pcmSizeBytes) == 0;

  return report.recordingFinished && report.durationValid &&
         report.sampleCountValid && report.pcmSizeValid && report.wavHeaderValid &&
         report.wavSizeValid && report.pcmPlacementValid;
}

bool RecorderVerification::validateWavHeader(const uint8_t *wavData,
                                             size_t wavSizeBytes) {
  if (wavData == nullptr || wavSizeBytes < kWavHeaderSize ||
      std::memcmp(wavData, "RIFF", 4) != 0 ||
      std::memcmp(wavData + 8, "WAVE", 4) != 0 ||
      std::memcmp(wavData + 12, "fmt ", 4) != 0 ||
      std::memcmp(wavData + 36, "data", 4) != 0) {
    return false;
  }

  const uint16_t channels = readUint16LE(wavData + 22);
  const uint16_t bitsPerSample = readUint16LE(wavData + 34);
  const uint32_t sampleRate = readUint32LE(wavData + 24);
  const uint32_t byteRate = readUint32LE(wavData + 28);
  const uint16_t blockAlign = readUint16LE(wavData + 32);
  const uint32_t dataSize = readUint32LE(wavData + 40);

  return readUint32LE(wavData + 4) == wavSizeBytes - 8 &&
         readUint32LE(wavData + 16) == 16 &&
         readUint16LE(wavData + 20) == 1 &&
         channels == Config::RECORD_CHANNELS &&
         sampleRate == Config::RECORD_SAMPLE_RATE &&
         bitsPerSample == Config::RECORD_BITS_PER_SAMPLE &&
         blockAlign == channels * (bitsPerSample / 8) &&
         byteRate == sampleRate * blockAlign &&
         dataSize == wavSizeBytes - kWavHeaderSize;
}

void RecorderVerification::printRecorderDiagnostics(
    const RecorderValidationReport &report) {
#if TUNESCOPE_ENABLE_RECORDER_VERIFICATION
  Serial.printf("Recorder verification: duration=%u ms, samples=%u, PCM=%u bytes, WAV=%u bytes.\n",
                static_cast<unsigned>(report.recordingDurationMs),
                static_cast<unsigned>(report.sampleCount),
                static_cast<unsigned>(report.pcmSizeBytes),
                static_cast<unsigned>(report.wavSizeBytes));
  Serial.printf("Recorder verification: finished=%s duration=%s samples=%s PCM=%s header=%s WAV=%s payload=%s.\n",
                report.recordingFinished ? "PASS" : "FAIL",
                report.durationValid ? "PASS" : "FAIL",
                report.sampleCountValid ? "PASS" : "FAIL",
                report.pcmSizeValid ? "PASS" : "FAIL",
                report.wavHeaderValid ? "PASS" : "FAIL",
                report.wavSizeValid ? "PASS" : "FAIL",
                report.pcmPlacementValid ? "PASS" : "FAIL");
#else
  (void)report;
#endif
}

void RecorderVerification::printWavInfo(const uint8_t *wavData,
                                        size_t wavSizeBytes) {
#if TUNESCOPE_ENABLE_RECORDER_VERIFICATION
  if (wavData == nullptr || wavSizeBytes < kWavHeaderSize) {
    Serial.println(F("WAV verification: invalid or empty buffer."));
    return;
  }

  Serial.printf("WAV info: size=%u, sample rate=%u, channels=%u, bits=%u, byte rate=%u, block align=%u.\n",
                static_cast<unsigned>(wavSizeBytes),
                static_cast<unsigned>(readUint32LE(wavData + 24)),
                static_cast<unsigned>(readUint16LE(wavData + 22)),
                static_cast<unsigned>(readUint16LE(wavData + 34)),
                static_cast<unsigned>(readUint32LE(wavData + 28)),
                static_cast<unsigned>(readUint16LE(wavData + 32)));
#else
  (void)wavData;
  (void)wavSizeBytes;
#endif
}

bool RecorderVerification::exportWavToSerial(const uint8_t *wavData,
                                              size_t wavSizeBytes) {
#if TUNESCOPE_ENABLE_RECORDER_VERIFICATION
  if (!validateWavHeader(wavData, wavSizeBytes)) {
    Serial.println(F("WAV export: validation failed."));
    return false;
  }

  Serial.printf("BEGIN_WAV %u\n", static_cast<unsigned>(wavSizeBytes));
  Serial.write(wavData, wavSizeBytes);
  Serial.flush();
  Serial.println();
  Serial.println(F("END_WAV"));
  return true;
#else
  (void)wavData;
  (void)wavSizeBytes;
  return false;
#endif
}
