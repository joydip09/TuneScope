#pragma once

#include <Arduino.h>

enum class RecognitionState {
  Idle,
  Recording,
  Uploading,
  Recognizing,
  SongFound,
  SongNotFound,
  WiFiError,
  UploadFailed,
  ApiError,
  Failed
};

struct SongInfo {
  bool found = false;

  String title;
  String artist;
  String album;
  String songLink;
  String statusMessage;
};