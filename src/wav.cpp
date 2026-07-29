#include "wav.h"

#include <cstring>
#include <limits>

#include <esp_heap_caps.h>

namespace {

constexpr uint16_t kPcmAudioFormat = 1;
constexpr uint32_t kFmtChunkSize = 16;

void writeUint16LE(uint8_t *destination, uint16_t value) {
  destination[0] = static_cast<uint8_t>(value & 0xFF);
  destination[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
}

void writeUint32LE(uint8_t *destination, uint32_t value) {
  destination[0] = static_cast<uint8_t>(value & 0xFF);
  destination[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
  destination[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
  destination[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
}

} // namespace

WavGenerator::~WavGenerator() { clear(); }

bool WavGenerator::build(const void *pcmData, size_t sampleCount,
                         uint32_t sampleRate, uint16_t channels,
                         uint16_t bitsPerSample) {
  clear();

  if (pcmData == nullptr || sampleCount == 0 || sampleRate == 0 ||
      channels == 0 || bitsPerSample == 0 || bitsPerSample % 8 != 0) {
    return false;
  }

  const size_t bytesPerSample = bitsPerSample / 8;
  if (sampleCount > std::numeric_limits<size_t>::max() / bytesPerSample) {
    return false;
  }

  const size_t pcmBytes = sampleCount * bytesPerSample;
  if (pcmBytes > std::numeric_limits<uint32_t>::max() ||
      pcmBytes > std::numeric_limits<size_t>::max() - kHeaderSizeBytes) {
    return false;
  }

  const uint32_t blockAlign = channels * bytesPerSample;
  if (blockAlign > std::numeric_limits<uint16_t>::max() ||
      sampleRate > std::numeric_limits<uint32_t>::max() / blockAlign) {
    return false;
  }

  const size_t wavBytes = kHeaderSizeBytes + pcmBytes;
  uint8_t *wavBuffer =
      static_cast<uint8_t *>(heap_caps_malloc(wavBytes, MALLOC_CAP_SPIRAM));
  if (wavBuffer == nullptr) {
    return false;
  }

  std::memcpy(wavBuffer + kHeaderSizeBytes, pcmData, pcmBytes);

  std::memcpy(wavBuffer + 0, "RIFF", 4);
  writeUint32LE(wavBuffer + 4,
                static_cast<uint32_t>(wavBytes - sizeof(uint32_t) * 2));
  std::memcpy(wavBuffer + 8, "WAVE", 4);
  std::memcpy(wavBuffer + 12, "fmt ", 4);
  writeUint32LE(wavBuffer + 16, kFmtChunkSize);
  writeUint16LE(wavBuffer + 20, kPcmAudioFormat);
  writeUint16LE(wavBuffer + 22, channels);
  writeUint32LE(wavBuffer + 24, sampleRate);
  writeUint32LE(wavBuffer + 28, sampleRate * blockAlign);
  writeUint16LE(wavBuffer + 32, static_cast<uint16_t>(blockAlign));
  writeUint16LE(wavBuffer + 34, bitsPerSample);
  std::memcpy(wavBuffer + 36, "data", 4);
  writeUint32LE(wavBuffer + 40, static_cast<uint32_t>(pcmBytes));

  m_data = wavBuffer;
  m_sizeBytes = wavBytes;
  m_pcmSizeBytes = pcmBytes;
  return true;
}

void WavGenerator::clear() {
  if (m_data != nullptr) {
    free(m_data);
    m_data = nullptr;
  }

  m_sizeBytes = 0;
  m_pcmSizeBytes = 0;
}

const uint8_t *WavGenerator::data() const { return m_data; }

size_t WavGenerator::sizeBytes() const { return m_sizeBytes; }

size_t WavGenerator::pcmSizeBytes() const { return m_pcmSizeBytes; }
