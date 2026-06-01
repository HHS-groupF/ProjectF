#ifndef NETWERKMANAGER_H
#define NETWERKMANAGER_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class NetwerkManager {
  private:
    const char* _ssid;
    const char* _password;
    const char* _mqttServer;
    WiFiClient _espClient;
    PubSubClient _client;
    String _clientId;                       
    unsigned long _laatsteHerverbindTijd = 0; 

    void setupWifi();
    void verbindMqtt(); 

  public:
    NetwerkManager(const char* ssid, const char* password, const char* mqttServer);

    void begin(MQTT_CALLBACK_SIGNATURE); 
    void loop();                         

    void stuurBericht(const char* topic, const char* bericht); 
    void subscribe(const char* topic);                         
    bool isConnected();                                       
    PubSubClient& getClient();
};

#endif
