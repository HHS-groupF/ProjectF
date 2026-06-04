#include "SocketWemos.h"

SocketWemos::SocketWemos(const char* ssid, const char* password,
                         const char* serverIp, uint16_t serverPoort)
  : _ssid(ssid), _password(password), _serverIp(serverIp), _serverPoort(serverPoort) {}

void SocketWemos::begin(RuneCallback callback) {
  _callback = callback;
  verbindWifi();
  verbindServer();
}

void SocketWemos::verbindWifi() {
  Serial.println();
  Serial.print("Verbinden met WiFi "); Serial.println(_ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi verbonden! IP: ");
  Serial.println(WiFi.localIP());
}

void SocketWemos::verbindServer() {
  Serial.print("[Bifrost] verbinden met Heimdall ");
  Serial.print(_serverIp); Serial.print(":"); Serial.println(_serverPoort);

  if (_client.connect(_serverIp, _serverPoort)) {
    _client.setNoDelay(true);   // geen Nagle-buffering: Runes meteen versturen
    Serial.println("[Bifrost] verbonden met Heimdall.");
  } else {
    Serial.println("[Bifrost] verbinden mislukt, opnieuw proberen...");
    _laatsteHerverbindTijd = millis();
  }
}

void SocketWemos::loop() {
  if (WiFi.status() != WL_CONNECTED) {
    verbindWifi();
  }

  if (!_client.connected()) {
    // Niet-blokkerende herverbind: één poging per 3 seconden.
    if (millis() - _laatsteHerverbindTijd >= 3000) verbindServer();
    return;
  }

  leesData();
}

void SocketWemos::leesData() {
  // Lees byte voor byte; een Rune is compleet bij een newline.
  while (_client.available()) {
    char c = (char)_client.read();

    if (c == '\n') {
      String regel = _ontvangBuffer;
      _ontvangBuffer = "";
      regel.trim();
      if (regel.length() == 0) continue;

      // Splits op de eerste spatie: topic + payload.
      int sp = regel.indexOf(' ');
      String topic   = (sp < 0) ? regel : regel.substring(0, sp);
      String payload = (sp < 0) ? String("") : regel.substring(sp + 1);

      if (_callback) {
        char topicBuf[64];
        topic.toCharArray(topicBuf, sizeof(topicBuf));
        _callback(topicBuf, (uint8_t*)payload.c_str(), payload.length());
      }
    } else if (c != '\r') {
      _ontvangBuffer += c;
    }
  }
}

void SocketWemos::stuurBericht(const char* topic, const char* bericht) {
  if (!_client.connected()) return;
  // Rune = "topic<spatie>payload\n"
  _client.print(topic);
  _client.print(' ');
  _client.print(bericht);
  _client.print('\n');
}

bool SocketWemos::isVerbonden() {
  return _client.connected() && WiFi.status() == WL_CONNECTED;
}
