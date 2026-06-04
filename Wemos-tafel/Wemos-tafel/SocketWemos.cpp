#include "SocketWemos.h"

// Rauwe lwIP BSD-sockets (onderdeel van de ESP8266-core, geen externe library).
#include <lwip/sockets.h>
#include <lwip/inet.h>
#include <errno.h>

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
  if (_sock >= 0) { lwip_close(_sock); _sock = -1; }

  _sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
  if (_sock < 0) {
    Serial.println("[Bifrost] socket() mislukt");
    _laatsteHerverbindTijd = millis();
    return;
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(_serverPoort);
  serverAddr.sin_addr.s_addr = inet_addr(_serverIp);

  if (lwip_connect(_sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    Serial.println("[Bifrost] verbinden met Heimdall mislukt, opnieuw proberen...");
    lwip_close(_sock);
    _sock = -1;
    _laatsteHerverbindTijd = millis();
    return;
  }

  // Zet de socket non-blocking zodat loop() niet blijft hangen op recv().
  int nonblocking = 1;
  lwip_ioctl(_sock, FIONBIO, &nonblocking);

  Serial.println("[Bifrost] verbonden met Heimdall.");
}

void SocketWemos::loop() {
  if (WiFi.status() != WL_CONNECTED) {
    verbindWifi();
  }

  if (_sock < 0) {
    // Niet-blokkerende herverbind: één poging per 3 seconden.
    if (millis() - _laatsteHerverbindTijd >= 3000) verbindServer();
    return;
  }

  leesData();
}

void SocketWemos::leesData() {
  uint8_t temp[256];
  int n = lwip_recv(_sock, temp, sizeof(temp), 0);

  if (n > 0) {
    for (int i = 0; i < n; i++) _ontvangBuffer += (char)temp[i];

    // Verwerk volledige regels (Runes) uit de buffer.
    int nl;
    while ((nl = _ontvangBuffer.indexOf('\n')) >= 0) {
      String regel = _ontvangBuffer.substring(0, nl);
      _ontvangBuffer = _ontvangBuffer.substring(nl + 1);
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
    }
  } else if (n == 0) {
    // Server heeft de verbinding gesloten.
    Serial.println("[Bifrost] verbinding gesloten door server.");
    lwip_close(_sock); _sock = -1; _laatsteHerverbindTijd = millis();
  } else {
    // n < 0: EWOULDBLOCK/EAGAIN = gewoon geen data; andere fout = verbinding kwijt.
    if (errno != EWOULDBLOCK && errno != EAGAIN) {
      lwip_close(_sock); _sock = -1; _laatsteHerverbindTijd = millis();
    }
  }
}

void SocketWemos::stuurBericht(const char* topic, const char* bericht) {
  if (_sock < 0) return;
  String rune = String(topic) + " " + String(bericht) + "\n";
  lwip_send(_sock, rune.c_str(), rune.length(), 0);
}

bool SocketWemos::isVerbonden() {
  return _sock >= 0 && WiFi.status() == WL_CONNECTED;
}
