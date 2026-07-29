#include "recognizer.h"

#include "config.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <esp_crt_bundle.h>

#include <cstdio>
#include <cstring>
#include <limits>

namespace {

constexpr char kMockTitle[] = "Believer";
constexpr char kMockArtist[] = "Imagine Dragons";
constexpr char kMockAlbum[] = "Evolve";
constexpr char kMockSongLink[] = "https://www.youtube.com/watch?v=7wtfhZwyrcc";

constexpr char kMultipartBoundary[] = "TuneScopeWavBoundary7MA4YWxkTrZu0gW";
constexpr char kMultipartContentTypePrefix[] = "multipart/form-data; boundary=";
constexpr char kMultipartPrefixFormat[] =
    "--%s\r\n"
    "Content-Disposition: form-data; name=\"api_token\"\r\n\r\n"
    "%s\r\n"
    "--%s\r\n"
    "Content-Disposition: form-data; name=\"file\"; "
    "filename=\"recording.wav\"\r\n"
    "Content-Type: audio/wav\r\n\r\n";
constexpr char kMultipartSuffix[] =
    "\r\n--TuneScopeWavBoundary7MA4YWxkTrZu0gW--\r\n";
constexpr size_t kMultipartPrefixCapacity = 512;
constexpr size_t kMultipartContentTypeCapacity = 256;

constexpr int kInvalidRequestStatus = -1;
constexpr int kMockModeStatus = -2;
constexpr int kSizeOverflowStatus = -3;
constexpr int kClientInitializationFailureStatus = -4;
constexpr size_t kJsonDocumentCapacity = 2048;

class MultipartStream : public Stream {
public:
  MultipartStream(const char *prefix, size_t prefixSize, const uint8_t *wavData,
                  size_t wavSize, const char *suffix, size_t suffixSize)
      : m_segments{reinterpret_cast<const uint8_t *>(prefix), wavData,
                   reinterpret_cast<const uint8_t *>(suffix)},
        m_segmentSizes{prefixSize, wavSize, suffixSize} {}

  int available() override {
    const size_t bytesRemaining = remainingBytes();
    const size_t maximumInt =
        static_cast<size_t>(std::numeric_limits<int>::max());
    return static_cast<int>(bytesRemaining > maximumInt ? maximumInt
                                                        : bytesRemaining);
  }

  int read() override {
    uint8_t byte = 0;
    return readBytes(reinterpret_cast<char *>(&byte), 1) == 1 ? byte : -1;
  }

  int peek() override {
    advanceEmptySegments();
    if (m_currentSegment == kSegmentCount) {
      return -1;
    }

    return m_segments[m_currentSegment][m_segmentOffset];
  }

  void flush() override {}

  size_t write(uint8_t) override { return 0; }

  size_t readBytes(char *buffer, size_t length) override {
    if (buffer == nullptr || length == 0) {
      return 0;
    }

    size_t copied = 0;
    while (copied < length) {
      advanceEmptySegments();
      if (m_currentSegment == kSegmentCount) {
        break;
      }

      const size_t segmentRemaining =
          m_segmentSizes[m_currentSegment] - m_segmentOffset;
      const size_t copySize = (length - copied) < segmentRemaining
                                  ? (length - copied)
                                  : segmentRemaining;
      std::memcpy(buffer + copied,
                  m_segments[m_currentSegment] + m_segmentOffset, copySize);
      copied += copySize;
      m_segmentOffset += copySize;
    }

    return copied;
  }

private:
  static constexpr size_t kSegmentCount = 3;

  void advanceEmptySegments() {
    while (m_currentSegment < kSegmentCount &&
           m_segmentOffset == m_segmentSizes[m_currentSegment]) {
      ++m_currentSegment;
      m_segmentOffset = 0;
    }
  }

  size_t remainingBytes() const {
    size_t total = 0;
    for (size_t index = m_currentSegment; index < kSegmentCount; ++index) {
      total += m_segmentSizes[index] -
               (index == m_currentSegment ? m_segmentOffset : 0);
    }
    return total;
  }

  const uint8_t *m_segments[kSegmentCount];
  size_t m_segmentSizes[kSegmentCount];
  size_t m_currentSegment = 0;
  size_t m_segmentOffset = 0;
};

bool hasWavData(const uint8_t *wavData, size_t wavSizeBytes) {
  return wavData != nullptr && wavSizeBytes > 0;
}

bool hasValidRequest(const HttpsUploadRequest &request) {
  return request.endpoint != nullptr && request.apiToken != nullptr &&
         request.endpoint[0] != '\0' && request.apiToken[0] != '\0';
}

bool buildMultipartPrefix(char *buffer, size_t bufferCapacity,
                          size_t &prefixSize, const char *boundary,
                          const char *apiToken) {
  const int prefixLength =
      std::snprintf(buffer, bufferCapacity, kMultipartPrefixFormat, boundary,
                    apiToken, boundary);
  if (prefixLength < 0) {
    return false;
  }

  prefixSize = static_cast<size_t>(prefixLength);
  return prefixSize < bufferCapacity;
}

bool buildMultipartContentType(char *buffer, size_t bufferCapacity,
                               const char *boundary) {
  const int contentTypeLength = std::snprintf(
      buffer, bufferCapacity, "%s%s", kMultipartContentTypePrefix, boundary);
  if (contentTypeLength < 0) {
    return false;
  }

  return static_cast<size_t>(contentTypeLength) < bufferCapacity;
}

void copyStringField(const JsonObject &object, const char *key,
                     String &target) {
  const JsonVariant value = object[key];
  if (value.is<const char *>()) {
    target = value.as<const char *>();
  }
}

} // namespace

bool Recognizer::begin() { return true; }

SongInfo Recognizer::parseSongInfoResponse(const String &responseBody) {
  SongInfo result{};

  if (responseBody.isEmpty()) {
    return result;
  }

  JsonDocument document;
  const DeserializationError error = deserializeJson(document, responseBody);
  if (error) {
    return result;
  }

  const char *status = document["status"];
  if (status == nullptr || std::strcmp(status, "success") != 0) {
    return result;
  }

  const JsonVariant resultVariant = document["result"];
  if (!resultVariant.is<JsonObject>()) {
    return result;
  }

  const JsonObject resultObject = resultVariant.as<JsonObject>();
  copyStringField(resultObject, "title", result.title);
  copyStringField(resultObject, "artist", result.artist);
  copyStringField(resultObject, "album", result.album);

  const JsonVariant songLinkValue = resultObject["song_link"];
  if (songLinkValue.is<const char *>()) {
    result.songLink = songLinkValue.as<const char *>();
  }

  if (result.songLink.isEmpty()) {
    const JsonVariant legacySongLinkValue = resultObject["songLink"];
    if (legacySongLinkValue.is<const char *>()) {
      result.songLink = legacySongLinkValue.as<const char *>();
    }
  }

  result.found = !result.title.isEmpty() || !result.artist.isEmpty() ||
                 !result.album.isEmpty() || !result.songLink.isEmpty();
  return result;
}

SongInfo Recognizer::recognize(const uint8_t *wavData, size_t wavSizeBytes) {
  SongInfo result{};

  if (!hasWavData(wavData, wavSizeBytes)) {
    return result;
  }

  if (Config::ENABLE_MOCK_RECOGNITION) {
    result.found = true;
    result.title = kMockTitle;
    result.artist = kMockArtist;
    result.album = kMockAlbum;
    result.songLink = kMockSongLink;
  }

  return result;
}

HttpsUploadResult Recognizer::uploadWav(const uint8_t *wavData,
                                        size_t wavSizeBytes,
                                        const HttpsUploadRequest &request) {
  HttpsUploadResult result{};

  if (!hasWavData(wavData, wavSizeBytes) || !hasValidRequest(request)) {
    result.httpStatusCode = kInvalidRequestStatus;
    result.errorMessage = "Invalid WAV data or HTTPS request parameters.";
    return result;
  }

  if (Config::ENABLE_MOCK_RECOGNITION) {
    result.httpStatusCode = kMockModeStatus;
    result.errorMessage = "Mock recognition is enabled.";
    return result;
  }

  char prefix[kMultipartPrefixCapacity];
  char contentType[kMultipartContentTypeCapacity];
  size_t prefixSize = 0;
  if (!buildMultipartPrefix(prefix, sizeof(prefix), prefixSize,
                            kMultipartBoundary, request.apiToken) ||
      !buildMultipartContentType(contentType, sizeof(contentType),
                                 kMultipartBoundary)) {
    result.httpStatusCode = kInvalidRequestStatus;
    result.errorMessage = "Unable to build the multipart request headers.";
    return result;
  }

  const size_t suffixSize = sizeof(kMultipartSuffix) - 1;
  if (wavSizeBytes > std::numeric_limits<size_t>::max() - prefixSize ||
      wavSizeBytes + prefixSize >
          std::numeric_limits<size_t>::max() - suffixSize) {
    result.httpStatusCode = kSizeOverflowStatus;
    result.errorMessage = "Multipart request exceeds the supported size.";
    return result;
  }

  const size_t contentLength = prefixSize + wavSizeBytes + suffixSize;
  MultipartStream requestBody(prefix, prefixSize, wavData, wavSizeBytes,
                              kMultipartSuffix, suffixSize);

  WiFiClientSecure secureClient;
  secureClient.setInsecure();

  HTTPClient http;
  if (!http.begin(secureClient, request.endpoint)) {
    result.httpStatusCode = kClientInitializationFailureStatus;
    result.errorMessage = "Failed to initialize the HTTPS client.";
    http.end();
    secureClient.stop();
    return result;
  }

  http.addHeader("Content-Type", contentType);
  const int responseCode =
      http.sendRequest("POST", &requestBody, contentLength);
  result.httpStatusCode = responseCode;

  if (responseCode < 0) {
    result.success = false;
    result.errorMessage = http.errorToString(responseCode);
  } else {
    result.success = responseCode >= HTTP_CODE_OK &&
                     responseCode < HTTP_CODE_MULTIPLE_CHOICES;
    if (!result.success) {
      result.errorMessage = "HTTPS upload returned a non-2xx response.";
    }
  }

  if (responseCode >= 0) {
    result.responseBody = http.getString();
    if (result.success) {
      result.songInfo = parseSongInfoResponse(result.responseBody);
    } else {
      result.songInfo = SongInfo{};
    }
  }

  http.end();
  secureClient.stop();
  return result;
}
