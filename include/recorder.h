#pragma once

#include <Arduino.h>

class Recorder {
public:
  Recorder();
  ~Recorder();

  Recorder(const Recorder &) = delete;
  Recorder &operator=(const Recorder &) = delete;

  // Checks that PSRAM is available. Audio::begin() must have succeeded first.
  bool begin();

  // Records exactly Config::RECORD_DURATION_SEC seconds of 16-bit PCM.
  // This call blocks only while collecting the configured audio frames.
  bool startRecording();

  bool isRecording() const;
  bool isFinished() const;

  // Releases the PSRAM recording buffer and resets the recorder to idle.
  void clear();

  const int16_t *pcmData() const;
  size_t sampleCount() const;
  size_t bufferSizeBytes() const;
  uint32_t recordingDurationMs() const;

private:
  enum class State { Idle, Recording, Finished };

  bool allocateBuffer();

  int16_t *m_buffer;
  size_t m_sampleCount;
  size_t m_bufferSizeBytes;
  uint32_t m_recordingDurationMs;
  bool m_initialized;
  State m_state;
};
