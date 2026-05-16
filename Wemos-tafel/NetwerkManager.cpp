#include "NetwerkManager.h"

NetwerkManager::NetwerkManager(const char* ssid, const char* password, const char* mqttServer) 
  : _ssid(ssid), _password(password), _mqttServer(mqttServer), _client(_espClient) {}

void NetwerkManager::begin(MQTT_CALLBACK_SIGNATURE) {
  setupWifi();
  _client.setServer(_mqttServer, 1883);
  _client.setCallback(callback);
}

void NetwerkManager::setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Verbinden met "); Serial.println(_ssid);

  // Als statisch IP is ingesteld in config.h, gebruik dat in plaats van DHCP
  #ifdef GEBRUIK_STATISCH_IP
    WiFi.config(IPAddress(STATISCH_IP), IPAddress(GATEWAY), IPAddress(SUBNET));
    Serial.println("Statisch IP ingesteld.");
  #endif

  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi verbonden!");
}

void NetwerkManager::loop() {
  if (!_client.connected()) {
    while (!_client.connected()) {
      Serial.print("MQTT verbinden...");
      if (_client.connect("WemosTafelUnit")) {
        Serial.println("verbonden");
        _client.subscribe("tafel/+/reset");
      } else {
        Serial.print("fout, rc="); Serial.print(_client.state());
        Serial.println(" probeer opnieuw over 5 seconden");
        delay(5000);
      }
    }
  }
  _client.loop();
}

void NetwerkManager::stuurBericht(const char* topic, const char* bericht) {
  _client.publish(topic, bericht);
}

PubSubClient& NetwerkManager::getClient() {
  return _client;
}