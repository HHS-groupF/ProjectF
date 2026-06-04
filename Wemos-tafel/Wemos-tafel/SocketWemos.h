#ifndef SOCKETWEMOS_H
#define SOCKETWEMOS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>   // nodig om te associeren met het WiFi-netwerk en een IP te krijgen

// Callback in PubSubClient-stijl (char* topic, byte* payload, length),
// zodat de bestaande .ino-callback vrijwel ongewijzigd blijft.
typedef void (*RuneCallback)(char* topic, uint8_t* payload, unsigned int length);

// ===========================================================================
//  SocketWemos — de Bifrost-client (Valkyrie-rol) op de Wemos.
//  Praat via rauwe lwIP BSD-sockets met Heimdall (de Pi) over TCP/IP.
//  Een Rune is één regel:  "topic<spatie>payload\n".
//  WiFi-associatie gebeurt via ESP8266WiFi (nodig voor een IP); de TCP-laag
//  zelf is een eigen implementatie met socket()/connect()/recv()/send().
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

    int _sock = -1;
    RuneCallback _callback = nullptr;
    String _ontvangBuffer;
    unsigned long _laatsteHerverbindTijd = 0;
};

#endif // SOCKETWEMOS_H
