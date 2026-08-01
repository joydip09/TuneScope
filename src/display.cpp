#include "display.h"

#include "config.h"
#include "pins.h"
#include "visualizer.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace {

Adafruit_SSD1306 display(Config::OLED_WIDTH, Config::OLED_HEIGHT, &Wire, -1);
bool displayReady = false;

constexpr int16_t kHeaderY = 0;
constexpr int16_t kDividerY = 10;
constexpr int16_t kTitleY = 14;
constexpr int16_t kTitleHeight = 16;
constexpr int16_t kArtistY = 32;
constexpr int16_t kAlbumY = 48;
constexpr int16_t kRowWidth = 128;
constexpr float kRmsMin = 300.0f;
constexpr float kRmsMax = 5000.0f;

float normalizeAmplitude(uint16_t rms) {
  const float normalized =
      (static_cast<float>(rms) - kRmsMin) / (kRmsMax - kRmsMin);
  return constrain(normalized, 0.0f, 1.0f);
}

RecognitionState lastRenderedState = RecognitionState::Idle;
String lastRenderedTitle;
bool hasRenderedSongDetails = false;

void drawHorizontalBar(int x, int y, int width, int height, uint8_t percent) {
  percent = constrain(percent, 0, 100);

  display.drawRect(x, y, width, height, SSD1306_WHITE);

  int fillWidth = ((width - 2) * percent) / 100;

  display.fillRect(x + 1, y + 1, fillWidth, height - 2, SSD1306_WHITE);
}

void drawCenteredText(const String &text, uint8_t textSize, int16_t y,
                      uint8_t color) {
  int16_t x1 = 0;
  int16_t y1 = 0;
  uint16_t width = 0;
  uint16_t height = 0;

  display.setTextSize(textSize);
  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  const int16_t x = (Config::OLED_WIDTH - width) / 2;
  display.setCursor(x, y);
  display.setTextColor(color);
  display.print(text);
}

uint16_t measureTextWidth(const String &text, uint8_t textSize) {
  display.setTextSize(textSize);
  int16_t x1 = 0;
  int16_t y1 = 0;
  uint16_t width = 0;
  uint16_t height = 0;

  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
  return width;
}

String fitTextToWidth(const String &text, uint8_t textSize, int16_t maxWidth) {
  if (text.length() == 0) {
    return String();
  }

  String result = text;
  while (result.length() > 0 && measureTextWidth(result, textSize) > maxWidth) {
    result.remove(result.length() - 1);
  }

  return result;
}

void drawStaticSongLayout(const String &artistText, const String &albumText) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, kHeaderY);
  display.println(F("NOW PLAYING"));

  display.drawFastHLine(0, kDividerY, kRowWidth, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, kArtistY);
  display.print(fitTextToWidth(artistText, 1, kRowWidth));

  display.setCursor(0, kAlbumY);
  display.print(fitTextToWidth(albumText, 1, kRowWidth));
}

String getVisibleTitleWindow(const String &titleText, int16_t startIndex,
                             int16_t maxWidth) {
  if (titleText.length() == 0) {
    return String();
  }

  const int16_t safeStart =
      constrain(startIndex, 0, static_cast<int16_t>(titleText.length()));
  String window;
  for (int16_t i = safeStart; i < static_cast<int16_t>(titleText.length());
       ++i) {
    const String candidate = titleText.substring(safeStart, i + 1);
    if (measureTextWidth(candidate, 2) > maxWidth) {
      break;
    }
    window = candidate;
  }

  if (window.length() == 0) {
    return titleText.substring(safeStart, safeStart + 1);
  }

  return window;
}

void drawTitleRegion(const String &titleText, int16_t windowStart) {
  display.fillRect(0, kTitleY, kRowWidth, kTitleHeight, SSD1306_BLACK);

  if (titleText.length() == 0) {
    return;
  }

  display.setTextWrap(false);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, kTitleY);

  const int16_t titleWidth =
      static_cast<int16_t>(measureTextWidth(titleText, 2));
  const bool needsScroll = titleWidth > kRowWidth;

  if (!needsScroll) {
    display.print(titleText);
    return;
  }

  const int16_t safeStart =
      constrain(windowStart, 0, static_cast<int16_t>(titleText.length() - 1));

  const String visibleText =
      getVisibleTitleWindow(titleText, safeStart, kRowWidth);

  display.print(visibleText);
}

} // namespace

bool Display::begin() {
  if (displayReady) {
    return true;
  }

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS)) {
    return false;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
  displayReady = true;

  return true;
}

void Display::clear() { display.clearDisplay(); }

void Display::update() { display.display(); }

void Display::showSplash() {
  if (!displayReady && !Display::begin()) {
    return;
  }

  display.clearDisplay();

  drawCenteredText("TuneScope", 2, 20, SSD1306_WHITE);
  drawCenteredText("Ready", 1, 40, SSD1306_WHITE);
  drawCenteredText("Press REC", 1, 52, SSD1306_WHITE);

  display.display();
}

bool Display::showSongDetails(RecognitionState state, const SongInfo &songInfo,
                              int16_t &scrollOffset,
                              int16_t &maxScrollDistance) {
  if (!displayReady && !Display::begin()) {
    return false;
  }

  const bool shouldClearScreen =
      state != RecognitionState::SongFound || !songInfo.found;

  if (shouldClearScreen) {
    display.clearDisplay();
  }

  switch (state) {
  case RecognitionState::SongFound:
    if (songInfo.found) {
      const String titleText = songInfo.title;
      const String artistText = songInfo.artist;
      const String albumText =
          songInfo.album.length() > 0 ? songInfo.album : "----";

      const bool requiresFullRedraw = !hasRenderedSongDetails ||
                                      state != lastRenderedState ||
                                      titleText != lastRenderedTitle;

      if (requiresFullRedraw) {
        drawStaticSongLayout(artistText, albumText);
        lastRenderedState = state;
        lastRenderedTitle = titleText;
        hasRenderedSongDetails = true;
      }

      const int16_t titleWidth =
          static_cast<int16_t>(measureTextWidth(titleText, 2));
      const bool shouldScroll = titleWidth > kRowWidth;

      int16_t windowChars = 0;
      if (shouldScroll) {
        String window;
        for (int16_t i = 0; i < static_cast<int16_t>(titleText.length()); ++i) {
          const String candidate = titleText.substring(0, i + 1);
          if (measureTextWidth(candidate, 2) > kRowWidth) {
            break;
          }
          window = candidate;
          ++windowChars;
        }
      }

      maxScrollDistance =
          shouldScroll
              ? max(0, static_cast<int16_t>(titleText.length()) - windowChars)
              : 0;

      if (shouldScroll && scrollOffset > maxScrollDistance) {
        scrollOffset = maxScrollDistance;
      }

      drawTitleRegion(titleText, scrollOffset);
      display.display();
      return shouldScroll;
    }

    drawCenteredText("Song Not Found", 1, 24, SSD1306_WHITE);
    break;

  case RecognitionState::SongNotFound:
    drawCenteredText("Song Not Found", 1, 24, SSD1306_WHITE);
    break;

  case RecognitionState::WiFiError:
    drawCenteredText("Wi-Fi Error", 1, 20, SSD1306_WHITE);
    drawCenteredText("Reconnect Wi-Fi", 1, 40, SSD1306_WHITE);
    break;

  case RecognitionState::UploadFailed:
    drawCenteredText("Upload Failed", 1, 24, SSD1306_WHITE);
    break;

  case RecognitionState::ApiError:
    drawCenteredText("API Error", 1, 24, SSD1306_WHITE);
    break;

  case RecognitionState::Failed:
    drawCenteredText("Recognition Failed", 1, 24, SSD1306_WHITE);
    break;

  case RecognitionState::Idle:
    drawCenteredText("No Song", 1, 20, SSD1306_WHITE);
    drawCenteredText("Press REC", 1, 40, SSD1306_WHITE);
    break;

  case RecognitionState::Recording:
  case RecognitionState::Uploading:
  case RecognitionState::Recognizing:
  default:
    drawCenteredText("Recording...", 1, 20, SSD1306_WHITE);
    drawCenteredText("Please wait", 1, 40, SSD1306_WHITE);
    break;
  }

  lastRenderedState = state;
  lastRenderedTitle = String();
  hasRenderedSongDetails = false;
  display.display();
  return false;
}

void Display::showVisualizer(uint16_t rms) {
  if (!displayReady && !Display::begin()) {
    return;
  }
  // Invalidate any cached song details rendering so stale pixels do not
  // persist when switching to the visualizer.
  hasRenderedSongDetails = false;
  lastRenderedTitle = String();

  const float normalizedAmplitude = normalizeAmplitude(rms);

  display.fillRect(0, 0, Config::OLED_WIDTH, Config::OLED_HEIGHT,
                   SSD1306_BLACK);
  Visualizer::draw(display, normalizedAmplitude);
  display.display();
}

void Display::showWiFiStatus(WiFiState state, IPAddress ip) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println("Wi-Fi");
  display.println();

  switch (state) {
  case WiFiState::CONNECTING:
    display.println("Connecting...");
    break;

  case WiFiState::CONNECTED:
    display.println("Connected");
    display.println(ip);
    break;

  case WiFiState::DISCONNECTED:
    display.println("Disconnected");
    break;
  }

  display.display();
}

void Display::showAudioLevel(uint16_t rms) {
  display.clearDisplay();

  Serial.print("RMS: ");
  Serial.println(rms);

  long level = map(rms, 500, 5000, 0, 100);
  level = constrain(level, 0L, 100L);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Audio Level");

  drawHorizontalBar(8, 20, 112, 12, static_cast<uint8_t>(level));

  display.setCursor(0, 42);
  display.print("RMS: ");
  display.println(rms);

  display.display();
}
