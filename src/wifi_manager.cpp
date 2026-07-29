#include "wifi_manager.h"

#include "config.h"
#include "secrets.h"

#include <Arduino.h>
#include <WiFi.h>

namespace {

WiFiState currentState = WiFiState::DISCONNECTED;

unsigned long connectStartTime = 0;
unsigned long lastRetryTime = 0;

void startConnection() {
  Serial.println("[WiFi] Connecting...");

  WiFi.disconnect(true);
  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  connectStartTime = millis();
  currentState = WiFiState::CONNECTING;
}

} // namespace

bool WiFiManager::begin() {
  WiFi.mode(WIFI_STA);

  startConnection();

  return true;
}

void WiFiManager::update() {

  if (WiFi.status() == WL_CONNECTED) {

    if (currentState != WiFiState::CONNECTED) {
      Serial.println("[WiFi] Connected!");
      Serial.print("[WiFi] IP: ");
      Serial.println(WiFi.localIP());
    }

    currentState = WiFiState::CONNECTED;
    return;
  }

  if (currentState == WiFiState::CONNECTED) {
    Serial.println("[WiFi] Disconnected.");
    currentState = WiFiState::DISCONNECTED;
    lastRetryTime = millis();
    return;
  }

  if (currentState == WiFiState::CONNECTING) {

    if (millis() - connectStartTime >= Config::WIFI_CONNECT_TIMEOUT_MS) {
      Serial.println("[WiFi] Connection timed out.");

      currentState = WiFiState::DISCONNECTED;
      lastRetryTime = millis();
    }

    return;
  }

  if (currentState == WiFiState::DISCONNECTED) {

    if (millis() - lastRetryTime >= Config::WIFI_RETRY_INTERVAL_MS) {
      startConnection();
    }
  }
}

bool WiFiManager::isConnected() { return currentState == WiFiState::CONNECTED; }

IPAddress WiFiManager::localIP() { return WiFi.localIP(); }

WiFiState WiFiManager::state() { return currentState; }