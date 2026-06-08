#ifndef SOCKETWEMOS_H
#define SOCKETWEMOS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>   // levert WiFi-associatie én WiFiClient (de TCP-transportlaag)

// Callback-signatuur (char* topic, byte* payload, length) — bewust eenvoudig
// gehouden zodat de .ino-callback klein en overzichtelijk blijft.
typedef void (*RuneCallback)(char* topic, uint8_t* payload, unsigned int length);

// ===========================================================================
//  SocketWemos — de Bifrost-client (Valkyrie-rol) op de Wemos.
//  Praat via TCP/IP met Heimdall (de Pi). Het Bifrost-protocol (de Rune-framing,
//  het splitsen van topic/payload) is volledig eigen code; WiFiClient levert
//  alleen de kale TCP-bytestream (de ESP8266-tegenhanger van een socket).
//  Een Rune is één regel:  "topic<spatie>payload\n".
// ===========================================================================
class SocketWemos {
  public:
    SocketWemos(const char* ssid, const char* password,
                const char* serverIp, uint16_t serverPoort);

    void begin(RuneCallback callback);                          // WiFi + verbinden met Heimdall
    void loop();                                                // onderhoudt verbinding, leest Runes
    void stuurBericht(const char* topic, const char* bericht);  // verstuur een Rune
    bool isVerbonden();

  private:
    void verbindWifi();
    void verbindServer();
    void leesData();

    const char* _ssid;
    const char* _password;
    const char* _serverIp;
    uint16_t _serverPoort;

    WiFiClient _client;
    RuneCallback _callback = nullptr;
    String _ontvangBuffer;
    unsigned long _laatsteHerverbindTijd = 0;
};

#endif // SOCKETWEMOS_H
