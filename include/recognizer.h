#pragma once

#include <cstddef>
#include <cstdint>

#include "types.h"

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
   * Parses an AudD-style JSON response body into a reusable SongInfo object.
   *
   * @param responseBody Raw JSON response body returned by the HTTPS upload.
   * @return Parsed recognition metadata, or a result with found set to false.
   */
  SongInfo parseSongInfoResponse(const String &responseBody);

  /**
   * Recognizes a complete, read-only WAV buffer.
   *
   * @param wavData Pointer to the first byte of the WAV file.
   * @param wavSizeBytes Number of bytes available at wavData.
   * @return Recognition metadata, or a result with found set to false.
   */
  SongInfo recognize(const uint8_t *wavData, size_t wavSizeBytes);

  /**
   * Sends a read-only WAV buffer in a multipart/form-data HTTPS request.
   *
   * The request includes api_token and file fields. A successful result is a
   * 2xx HTTP response only; the response body is intentionally not processed.
   * No transaction is attempted while mock recognition is enabled.
   *
   * @param wavData Pointer to the first byte of the complete WAV file.
   * @param wavSizeBytes Number of bytes available at wavData.
   * @param request HTTPS endpoint, token, and optional server root CA. When no
   * root CA is supplied, the ESP32 certificate bundle is used.
   * @return HTTP transaction status without interpreting the response body.
   */
  HttpsUploadResult uploadWav(const uint8_t *wavData, size_t wavSizeBytes,
                              const HttpsUploadRequest &request);
};
