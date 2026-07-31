#pragma once

#include "recognizer.h"
#include "types.h"

enum class DisplayMode { Splash, Visualizer, SongDetails };

class DisplayManager : public RecognizerObserver {
public:
  DisplayManager();

  void setDisplayMode(DisplayMode mode);
  void nextDisplayMode();
  void setRecognitionState(RecognitionState state);
  void setSongInfo(const SongInfo &songInfo);
  void update();
  void render();

  void onRecognitionStateChanged(RecognitionState state) override;
  void onSongInfoUpdated(const SongInfo &songInfo) override;

private:
  enum class TitleScrollDirection { Forward, Reverse };

  static constexpr unsigned long kScrollStepIntervalMs = 500;

  DisplayMode displayMode_;
  RecognitionState recognitionState_;
  RecognitionState previousRecognitionState_;
  SongInfo songInfo_;
  bool needsRender_;
  bool scrollActive_;
  int16_t scrollOffset_;
  unsigned long lastScrollUpdateMs_;
  int16_t maxScrollDistance_;
  String lastTitle_;
  TitleScrollDirection titleScrollDirection_;
  bool clearDisplayOnNextRender_;
  unsigned long lastVisualizerUpdateMs_;

  void renderSplash();
  void renderVisualizer();
  void renderSongDetails();
  void resetTitleScroll(bool force = false);
};
