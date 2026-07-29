#include "audio.h"

#include "config.h"
#include "pins.h"

#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

namespace {

constexpr i2s_port_t I2S_PORT = I2S_NUM_0;
constexpr size_t SAMPLE_COUNT = 512;

int16_t sampleBuffer[SAMPLE_COUNT];
uint16_t rms = 0;

} // namespace

bool Audio::begin() {
  i2s_config_t config = {};

  config.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX);

  config.sample_rate = Config::SAMPLE_RATE;

  config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;

  config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;

  config.communication_format = I2S_COMM_FORMAT_STAND_I2S;

  config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;

  config.dma_buf_count = 8;
  config.dma_buf_len = 512;

  config.use_apll = false;

  config.tx_desc_auto_clear = false;

  config.fixed_mclk = 0;

  i2s_pin_config_t pins = {};

  pins.bck_io_num = MIC_SCK_PIN;
  pins.ws_io_num = MIC_WS_PIN;
  pins.data_out_num = I2S_PIN_NO_CHANGE;
  pins.data_in_num = MIC_SD_PIN;

  if (i2s_driver_install(I2S_PORT, &config, 0, nullptr) != ESP_OK)
    return false;

  if (i2s_set_pin(I2S_PORT, &pins) != ESP_OK)
    return false;

  i2s_zero_dma_buffer(I2S_PORT);

  return true;
}

void Audio::update() {
  size_t bytesRead = 0;

  if (i2s_read(I2S_PORT, sampleBuffer, sizeof(sampleBuffer), &bytesRead,
               portMAX_DELAY) != ESP_OK) {
    return;
  }

  const size_t count = bytesRead / sizeof(int16_t);

  if (count == 0)
    return;

  uint64_t sum = 0;

  for (size_t i = 0; i < count; i++) {
    int32_t sample = sampleBuffer[i];

    sum += static_cast<uint64_t>(sample * sample);
  }

  rms = static_cast<uint16_t>(
      sqrt(static_cast<float>(sum) / static_cast<float>(count)));

  if (Config::DEBUG_SERIAL) {
    Serial.print("RMS: ");
    Serial.println(rms);
  }
}

uint16_t Audio::getRMS() { return rms; }

bool Audio::readSamples(int16_t *samples, size_t requestedSamples,
                        size_t &samplesRead, uint32_t timeoutMs) {
  samplesRead = 0;

  if (samples == nullptr || requestedSamples == 0) {
    return false;
  }

  size_t bytesRead = 0;
  const TickType_t timeoutTicks = pdMS_TO_TICKS(timeoutMs);
  const esp_err_t result =
      i2s_read(I2S_PORT, samples, requestedSamples * sizeof(int16_t),
               &bytesRead, timeoutTicks);

  samplesRead = bytesRead / sizeof(int16_t);
  return result == ESP_OK;
}
