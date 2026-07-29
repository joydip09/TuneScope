#include <Arduino.h>
#include <unity.h>

#include "wav.h"

// PlatformIO excludes src/ from embedded unit-test builds by default.
#include "../../src/wav.cpp"

namespace {

constexpr uint32_t kSampleRate = 16000;
constexpr uint16_t kChannels = 1;
constexpr uint16_t kBitsPerSample = 16;
constexpr int16_t kPcmSamples[] = {0x1234, -0x1234, 0x0102, -0x0102};

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

void test_pcm_wav_header_and_payload() {
  WavGenerator wav;
  const size_t pcmBytes = sizeof(kPcmSamples);

  TEST_ASSERT_TRUE(wav.build(kPcmSamples,
                             sizeof(kPcmSamples) / sizeof(kPcmSamples[0]),
                             kSampleRate, kChannels, kBitsPerSample));
  TEST_ASSERT_EQUAL_UINT32(WavGenerator::kHeaderSizeBytes, 44);
  TEST_ASSERT_EQUAL_UINT32(WavGenerator::kHeaderSizeBytes + pcmBytes,
                           wav.sizeBytes());
  TEST_ASSERT_EQUAL_UINT32(pcmBytes, wav.pcmSizeBytes());

  const uint8_t *data = wav.data();
  TEST_ASSERT_NOT_NULL(data);
  TEST_ASSERT_EQUAL_MEMORY("RIFF", data, 4);
  TEST_ASSERT_EQUAL_UINT32(wav.sizeBytes() - 8, readUint32LE(data + 4));
  TEST_ASSERT_EQUAL_MEMORY("WAVE", data + 8, 4);
  TEST_ASSERT_EQUAL_MEMORY("fmt ", data + 12, 4);
  TEST_ASSERT_EQUAL_UINT16(1, readUint16LE(data + 20));
  TEST_ASSERT_EQUAL_UINT16(kChannels, readUint16LE(data + 22));
  TEST_ASSERT_EQUAL_UINT32(kSampleRate, readUint32LE(data + 24));
  TEST_ASSERT_EQUAL_UINT32(kSampleRate * kChannels * (kBitsPerSample / 8),
                           readUint32LE(data + 28));
  TEST_ASSERT_EQUAL_UINT16(kBitsPerSample, readUint16LE(data + 34));
  TEST_ASSERT_EQUAL_MEMORY("data", data + 36, 4);
  TEST_ASSERT_EQUAL_UINT32(pcmBytes, readUint32LE(data + 40));
  TEST_ASSERT_EQUAL_MEMORY(kPcmSamples, data + WavGenerator::kHeaderSizeBytes,
                           pcmBytes);

  Serial.printf("WAV test: header=%u PCM=%u WAV=%u rate=%u channels=%u bits=%u.\n",
                static_cast<unsigned>(WavGenerator::kHeaderSizeBytes),
                static_cast<unsigned>(pcmBytes),
                static_cast<unsigned>(wav.sizeBytes()),
                static_cast<unsigned>(kSampleRate),
                static_cast<unsigned>(kChannels),
                static_cast<unsigned>(kBitsPerSample));
}

} // namespace

void setup() {
  Serial.begin(115200);
  delay(1000);
  UNITY_BEGIN();
  RUN_TEST(test_pcm_wav_header_and_payload);
  UNITY_END();
}

void loop() {}
