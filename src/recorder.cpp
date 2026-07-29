#include "recorder.h"

#include "audio.h"
#include "config.h"

#include <esp_heap_caps.h>

namespace {

constexpr size_t kExpectedSampleCount =
    static_cast<size_t>(Config::RECORD_SAMPLE_RATE) *
    Config::RECORD_DURATION_SEC;
constexpr size_t kExpectedBufferSizeBytes =
    kExpectedSampleCount * sizeof(int16_t);
constexpr uint32_t kReadTimeoutMs = 1000;

static_assert(Config::RECORD_SAMPLE_RATE == Config::SAMPLE_RATE,
              "Recorder and audio driver sample rates must match");
static_assert(Config::RECORD_CHANNELS == 1,
              "Recorder supports mono PCM only");
static_assert(Config::RECORD_BITS_PER_SAMPLE == 16,
              "Recorder supports 16-bit PCM only");

void logDiagnostic(const __FlashStringHelper *message) {
  if (Config::DEBUG_SERIAL) {
    Serial.println(message);
  }
}

} // namespace

Recorder::Recorder()
    : m_buffer(nullptr), m_sampleCount(0), m_bufferSizeBytes(0),
      m_recordingDurationMs(0), m_initialized(false), m_state(State::Idle) {}

Recorder::~Recorder() { clear(); }

bool Recorder::begin() {
  m_initialized = psramFound();

  if (!m_initialized) {
    logDiagnostic(F("Recorder: PSRAM was not detected."));
    return false;
  }

  if (Config::DEBUG_SERIAL) {
    Serial.printf("Recorder: PSRAM detected; recording size: %u bytes.\n",
                  static_cast<unsigned>(kExpectedBufferSizeBytes));
  }

  return true;
}

bool Recorder::allocateBuffer() {
  clear();

  m_buffer = static_cast<int16_t *>(
      heap_caps_malloc(kExpectedBufferSizeBytes, MALLOC_CAP_SPIRAM));
  if (m_buffer == nullptr) {
    logDiagnostic(F("Recorder: PSRAM allocation failed."));
    return false;
  }

  m_bufferSizeBytes = kExpectedBufferSizeBytes;

  if (Config::DEBUG_SERIAL) {
    Serial.printf("Recorder: allocated %u bytes in PSRAM.\n",
                  static_cast<unsigned>(m_bufferSizeBytes));
  }

  return true;
}

bool Recorder::startRecording() {
  if (!m_initialized && !begin()) {
    return false;
  }

  if (m_state == State::Recording || !allocateBuffer()) {
    return false;
  }

  m_state = State::Recording;
  const uint32_t recordingStartedAtMs = millis();

  if (Config::DEBUG_SERIAL) {
    Serial.printf("Recorder: recording started; buffer: %p, size: %u bytes.\n",
                  static_cast<void *>(m_buffer),
                  static_cast<unsigned>(m_bufferSizeBytes));
  }

  while (m_sampleCount < kExpectedSampleCount) {
    const size_t samplesRemaining = kExpectedSampleCount - m_sampleCount;
    size_t samplesRead = 0;

    if (!Audio::readSamples(m_buffer + m_sampleCount, samplesRemaining,
                            samplesRead, kReadTimeoutMs) ||
        samplesRead == 0) {
      logDiagnostic(F("Recorder: microphone read failed or timed out."));
      clear();
      return false;
    }

    m_sampleCount += samplesRead;
  }

  m_recordingDurationMs = millis() - recordingStartedAtMs;
  m_state = State::Finished;

  if (Config::DEBUG_SERIAL) {
    Serial.printf("Recorder: recording completed; %u samples, %u bytes, %u ms.\n",
                  static_cast<unsigned>(m_sampleCount),
                  static_cast<unsigned>(m_bufferSizeBytes),
                  static_cast<unsigned>(m_recordingDurationMs));
  }

  return true;
}

void Recorder::clear() {
  if (m_buffer != nullptr) {
    free(m_buffer);
    m_buffer = nullptr;
  }

  m_sampleCount = 0;
  m_bufferSizeBytes = 0;
  m_recordingDurationMs = 0;
  m_state = State::Idle;
}

bool Recorder::isRecording() const { return m_state == State::Recording; }

bool Recorder::isFinished() const { return m_state == State::Finished; }

const int16_t *Recorder::pcmData() const { return m_buffer; }

size_t Recorder::sampleCount() const { return m_sampleCount; }

size_t Recorder::bufferSizeBytes() const {
  return m_bufferSizeBytes;
}

uint32_t Recorder::recordingDurationMs() const {
  return m_recordingDurationMs;
}
