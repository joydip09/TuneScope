#pragma once

#include <stddef.h>
#include <stdint.h>

class Audio {
public:
  static bool begin();

  static void update();

  static uint16_t getRMS();

  // Reads PCM frames using the I2S driver initialized by begin().
  // samplesRead may be less than requestedSamples when the read times out.
  static bool readSamples(int16_t *samples, size_t requestedSamples,
                          size_t &samplesRead, uint32_t timeoutMs);
};
