#pragma once

#include <cstddef>
#include <cstdint>

class Recorder;
class WavGenerator;

struct RecorderValidationReport {
  bool recordingFinished = false;
  bool durationValid = false;
  bool sampleCountValid = false;
  bool pcmSizeValid = false;
  bool wavHeaderValid = false;
  bool wavSizeValid = false;
  bool pcmPlacementValid = false;

  uint32_t recordingDurationMs = 0;
  size_t sampleCount = 0;
  size_t pcmSizeBytes = 0;
  size_t wavSizeBytes = 0;
};

class RecorderVerification {
public:
  static bool validate(const Recorder &recorder, const WavGenerator &wav,
                       RecorderValidationReport &report);
  static bool validateWavHeader(const uint8_t *wavData, size_t wavSizeBytes);

  static void printRecorderDiagnostics(const RecorderValidationReport &report);
  static void printWavInfo(const uint8_t *wavData, size_t wavSizeBytes);

  // Enabled only when TUNESCOPE_ENABLE_RECORDER_VERIFICATION is set to 1.
  // Emits "BEGIN_WAV <byte-count>" followed by exactly byte-count binary
  // bytes, then "END_WAV". Capture the binary section on a host computer.
  static bool exportWavToSerial(const uint8_t *wavData, size_t wavSizeBytes);
};
