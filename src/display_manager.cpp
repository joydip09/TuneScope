#include "display_manager.h"

#include "audio.h"
#include "config.h"
#include "display.h"

#include <Arduino.h>

DisplayManager::DisplayManager()
    : displayMode_(DisplayMode::Splash),
      recognitionState_(RecognitionState::Idle),
      previousRecognitionState_(RecognitionState::Idle), songInfo_(),
      needsRender_(true), scrollActive_(false), scrollOffset_(0),
      lastScrollUpdateMs_(0), maxScrollDistance_(0), lastTitle_(),
      titleScrollDirection_(TitleScrollDirection::Forward),
      clearDisplayOnNextRender_(false), lastVisualizerUpdateMs_(0) {}

void DisplayManager::setDisplayMode(DisplayMode mode) {
  if (displayMode_ != mode) {
    Serial.printf("[UI] DisplayMode changed -> %d\n", static_cast<int>(mode));
    displayMode_ = mode;
    resetTitleScroll(true);
  }
}

void DisplayManager::onRecognitionStateChanged(RecognitionState state) {
  previousRecognitionState_ = recognitionState_;
  recognitionState_ = state;
  Serial.printf("[UI] RecognitionState changed -> %d\n",
                static_cast<int>(state));
  if (state != RecognitionState::Idle) {
    displayMode_ = DisplayMode::SongDetails;
  }
  clearDisplayOnNextRender_ =
      (state == RecognitionState::Recording &&
       previousRecognitionState_ != RecognitionState::Recording);
  resetTitleScroll(true);
}

void DisplayManager::onSongInfoUpdated(const SongInfo &songInfo) {
  songInfo_ = songInfo;
  Serial.printf("[UI] SongInfo updated: %s / %s\n", songInfo.title.c_str(),
                songInfo.artist.c_str());
  displayMode_ = DisplayMode::SongDetails;
  resetTitleScroll(true);
}

void DisplayManager::nextDisplayMode() {
  switch (displayMode_) {
  case DisplayMode::Splash:
    displayMode_ = DisplayMode::Visualizer;
    break;
  case DisplayMode::Visualizer:
    displayMode_ = DisplayMode::SongDetails;
    break;
  case DisplayMode::SongDetails:
    displayMode_ = DisplayMode::Splash;
    break;
  }

  resetTitleScroll(true);
}

void DisplayManager::setRecognitionState(RecognitionState state) {
  previousRecognitionState_ = recognitionState_;
  recognitionState_ = state;
  Serial.printf("[UI] RecognitionState set -> %d\n", static_cast<int>(state));
  if (state != RecognitionState::Idle) {
    displayMode_ = DisplayMode::SongDetails;
  }
  clearDisplayOnNextRender_ =
      (state == RecognitionState::Recording &&
       previousRecognitionState_ != RecognitionState::Recording);
  resetTitleScroll(true);
}

void DisplayManager::setSongInfo(const SongInfo &songInfo) {
  songInfo_ = songInfo;
  displayMode_ = DisplayMode::SongDetails;
  resetTitleScroll(true);
}

void DisplayManager::update() {
  const unsigned long now = millis();

  if (displayMode_ == DisplayMode::SongDetails) {
    const bool titleChanged = (songInfo_.title != lastTitle_);
    if (titleChanged) {
      resetTitleScroll(true);
      lastTitle_ = songInfo_.title;
      needsRender_ = true;
    }

    if (scrollActive_ && (now - lastScrollUpdateMs_) >= kScrollStepIntervalMs) {
      if (titleScrollDirection_ == TitleScrollDirection::Forward) {
        if (scrollOffset_ >= maxScrollDistance_) {
          scrollOffset_ = maxScrollDistance_;
          titleScrollDirection_ = TitleScrollDirection::Reverse;
        } else {
          ++scrollOffset_;
        }
      } else {
        if (scrollOffset_ <= 0) {
          scrollOffset_ = 0;
          titleScrollDirection_ = TitleScrollDirection::Forward;
        } else {
          --scrollOffset_;
        }
      }

      lastScrollUpdateMs_ = now;
      needsRender_ = true;
    }
  }

  if (displayMode_ == DisplayMode::Visualizer &&
      (now - lastVisualizerUpdateMs_) >= Config::DISPLAY_REFRESH_MS) {
    // Update audio samples for visualizer only when idle (do not interfere
    // with recording/recognition which consume audio buffers).
    if (recognitionState_ == RecognitionState::Idle) {
      Audio::update();
    }

    lastVisualizerUpdateMs_ = now;
    needsRender_ = true;
  }

  if (needsRender_) {
    render();
    needsRender_ = false;
  }
}

void DisplayManager::render() {
  if (clearDisplayOnNextRender_) {
    Display::clear();
    clearDisplayOnNextRender_ = false;
  }

  switch (displayMode_) {
  case DisplayMode::Splash:
    renderSplash();
    break;
  case DisplayMode::Visualizer:
    renderVisualizer();
    break;
  case DisplayMode::SongDetails:
    renderSongDetails();
    break;
  }
}

void DisplayManager::renderSplash() { Display::showSplash(); }

void DisplayManager::renderVisualizer() {
  if (recognitionState_ == RecognitionState::Recording ||
      recognitionState_ == RecognitionState::Uploading ||
      recognitionState_ == RecognitionState::Recognizing) {
    renderSongDetails();
    return;
  }
  Display::showVisualizer(Audio::getRMS());
}

void DisplayManager::renderSongDetails() {
  int16_t maxScrollDistance = 0;

  // If recognition has finished and the recognizer notified Idle after
  // reporting a terminal state (e.g. SongFound or SongNotFound), prefer
  // showing that terminal state so the user sees the results. The
  // recognizer sends Idle to indicate the cycle completed, but rendering
  // should still reflect the most recent terminal state.
  RecognitionState displayState = recognitionState_;
  if (recognitionState_ == RecognitionState::Idle &&
      previousRecognitionState_ != RecognitionState::Idle) {
    // Only map non-transient previous states through; transient states
    // like Recording/Uploading/Recognizing should not be used for final
    // result rendering.
    switch (previousRecognitionState_) {
    case RecognitionState::SongFound:
    case RecognitionState::SongNotFound:
    case RecognitionState::UploadFailed:
    case RecognitionState::ApiError:
    case RecognitionState::WiFiError:
    case RecognitionState::Failed:
      displayState = previousRecognitionState_;
      break;
    default:
      break;
    }
  }

  const bool shouldScroll = Display::showSongDetails(
      displayState, songInfo_, scrollOffset_, maxScrollDistance);

  maxScrollDistance_ = maxScrollDistance;
  scrollActive_ = shouldScroll;

  if (!shouldScroll) {
    scrollOffset_ = 0;
    titleScrollDirection_ = TitleScrollDirection::Forward;
    return;
  }

  if (scrollOffset_ > maxScrollDistance_) {
    scrollOffset_ = maxScrollDistance_;
  }
}

void DisplayManager::resetTitleScroll(bool force) {
  if (displayMode_ != DisplayMode::SongDetails && !force) {
    return;
  }

  scrollActive_ = false;
  scrollOffset_ = 0;
  lastScrollUpdateMs_ = millis();
  maxScrollDistance_ = 0;
  titleScrollDirection_ = TitleScrollDirection::Forward;
  needsRender_ = true;
}
