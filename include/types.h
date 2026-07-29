#pragma once

#include <Arduino.h>

enum class RecognitionState {
  Idle,
  Recording,
  Uploading,
  Recognizing,
  Success,
  NotFound,
  Error
};

struct SongInfo {
  bool found = false;

  String title;
  String artist;
  String album;

  String songLink;
};