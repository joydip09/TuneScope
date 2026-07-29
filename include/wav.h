#pragma once

#include <cstddef>
#include <cstdint>

class WavGenerator {
public:
  static constexpr size_t kHeaderSizeBytes = 44;

  WavGenerator() = default;
  ~WavGenerator();

  WavGenerator(const WavGenerator &) = delete;
  WavGenerator &operator=(const WavGenerator &) = delete;

  // sampleCount is the number of interleaved PCM samples, not bytes.
  bool build(const void *pcmData, size_t sampleCount, uint32_t sampleRate,
             uint16_t channels, uint16_t bitsPerSample);

  void clear();

  const uint8_t *data() const;
  size_t sizeBytes() const;
  size_t pcmSizeBytes() const;

private:
  uint8_t *m_data = nullptr;
  size_t m_sizeBytes = 0;
  size_t m_pcmSizeBytes = 0;
};
