#ifndef NETWERKMANAGER_H
#define NETWERKMANAGER_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Beheert de WiFi- en MQTT-verbinding voor de Wemos
class NetwerkManager {
  private:
    const char* _ssid;
    const char* _password;
    const char* _mqttServer;
    WiFiClient _espClient;
    PubSubClient _client;
    String _clientId;                        // Unieke ID gebaseerd op MAC-adres
    unsigned long _laatsteHerverbindTijd = 0; // Tijdstip van laatste MQTT herverbindpoging

    void setupWifi();
    void verbindMqtt(); // Eenmalige MQTT verbindpoging

  public:
    NetwerkManager(const char* ssid, const char* password, const char* mqttServer);

    void begin(MQTT_CALLBACK_SIGNATURE); // Initialiseert alles
    void loop();                         // Houdt WiFi en MQTT verbinding levend

    void stuurBericht(const char* topic, const char* bericht); // Publiceert een bericht op een topic
    void subscribe(const char* topic);                          // Abonneer op een extra topic
    bool isConnected();                                         // Geeft true als MQTT verbonden is
    PubSubClient& getClient();
};

#endif
