#pragma once

#include <cstddef>
#include <cstdint>

#include "recorder.h"
#include "types.h"
#include "wav.h"

/** HTTPS configuration for a single multipart WAV upload. */
struct HttpsUploadRequest {
  const char *endpoint = nullptr;
  const char *apiToken = nullptr;
  const char *rootCaCertificate = nullptr;
};

/** Result of an HTTPS upload transaction. */
struct HttpsUploadResult {
  bool success = false;
  int httpStatusCode = 0;
  String errorMessage;
  String responseBody;
  SongInfo songInfo;
};

class Recognizer {
public:
  /** Initializes the recognition backend. */
  bool begin();

  /**
   * Performs the complete recognition workflow: record audio, generate WAV,
   * upload it, parse the response, and return SongInfo.
   */
  SongInfo recognize();

private:
  bool recordAudio();
  bool generateWav();
  SongInfo parseSongInfoResponse(const String &responseBody,
                                 String &statusMessage);
  HttpsUploadResult uploadWav(const uint8_t *wavData, size_t wavSizeBytes,
                              const HttpsUploadRequest &request);

private:
  Recorder m_recorder;
  WavGenerator m_wav;
  bool m_initialized = false;
};
