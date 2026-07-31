#include "visualizer.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

float Visualizer::smoothedAmplitude_ = 0.0f;

const float Visualizer::kProfileMultiplier[kBarsPerSide] = {
    0.20f, 0.35f, 0.55f, 0.75f, 0.95f, 0.75f, 0.55f, 0.35f};

void Visualizer::draw(Adafruit_SSD1306 &display, float normalizedAmplitude) {
  smoothedAmplitude_ = smoothedAmplitude_ * (1.0f - kSmoothingFactor) +
                       normalizedAmplitude * kSmoothingFactor;

  const float amplitude = constrain(smoothedAmplitude_, 0.0f, 1.0f);
  const bool isSilent = amplitude < kSilenceThreshold;

  if (isSilent) {
    drawBaselineDots(display);
    return;
  }

  drawWaveform(display, amplitude);
}

void Visualizer::drawBaselineDots(Adafruit_SSD1306 &display) {
  constexpr int16_t dotCount = 12;
  constexpr int16_t dotStep = 8;
  constexpr int16_t dotRadius = 1;
  const int16_t centerY = kCenterY;
  const int16_t startX = kCenterX - ((dotCount - 1) * dotStep) / 2;
  const uint8_t phase = (millis() / 120) % dotStep;

  for (int16_t index = 0; index < dotCount; ++index) {
    const int16_t x = startX + index * dotStep;
    const int16_t y = centerY + ((index % 2 == 0) ? -1 : 1) *
                                    ((phase < (dotStep / 2)) ? 1 : -1);
    display.drawPixel(x, y, SSD1306_WHITE);
  }
}

void Visualizer::drawWaveform(Adafruit_SSD1306 &display, float amplitude) {
  const int16_t totalBars = kBarsPerSide * 2;
  const int16_t contentWidth =
      totalBars * kBarWidth + (totalBars - 1) * kBarGap;
  const int16_t startX = kCenterX - contentWidth / 2;

  for (int16_t side = 0; side < 2; ++side) {
    for (int16_t index = 0; index < kBarsPerSide; ++index) {
      const int16_t relativeHeight =
          kBarMinHeight +
          static_cast<int16_t>(amplitude * (kBarMaxHeight - kBarMinHeight) *
                               kProfileMultiplier[index]);
      const int16_t x = startX + (side == 0 ? index : (totalBars - 1 - index)) *
                                     (kBarWidth + kBarGap);
      const int16_t y = kCenterY - (relativeHeight / 2);

      display.fillRoundRect(x, y, kBarWidth, relativeHeight, 1, SSD1306_WHITE);
    }
  }
}
