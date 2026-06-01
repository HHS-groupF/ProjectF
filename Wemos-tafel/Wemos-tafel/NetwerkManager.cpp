#include "NetwerkManager.h"

NetwerkManager::NetwerkManager(const char* ssid, const char* password, const char* mqttServer)
  : _ssid(ssid), _password(password), _mqttServer(mqttServer), _client(_espClient) {}

void NetwerkManager::begin(MQTT_CALLBACK_SIGNATURE) {
  setupWifi();
  _clientId = "Tafel_" + WiFi.macAddress();
  _client.setServer(_mqttServer, 1883);
  _client.setCallback(callback);
  verbindMqtt();
}

void NetwerkManager::setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Verbinden met "); Serial.println(_ssid);

  
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

void NetwerkManager::verbindMqtt() {
  Serial.print("MQTT verbinden als "); Serial.print(_clientId); Serial.print("...");
  if (_client.connect(_clientId.c_str())) {
    Serial.println("verbonden");
    
    _client.subscribe("tafel/+/reset");      
    _client.subscribe("wemos/lichtkrant");   
    _client.subscribe("sensor/rgb/set");     
    
  } else {
    Serial.print("fout, rc="); Serial.print(_client.state());
    Serial.println(" probeer opnieuw over 5 seconden");
  }
  _laatsteHerverbindTijd = millis();
}

void NetwerkManager::loop() {
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi weg, opnieuw verbinden...");
    setupWifi();
  }


  if (!_client.connected()) {
    if (millis() - _laatsteHerverbindTijd >= 5000) {
      verbindMqtt();
    }
    return; 
  }

  _client.loop();
}

void NetwerkManager::stuurBericht(const char* topic, const char* bericht) {
  _client.publish(topic, bericht);
}

void NetwerkManager::subscribe(const char* topic) {
  _client.subscribe(topic);
}

bool NetwerkManager::isConnected() {
  return _client.connected();
}

PubSubClient& NetwerkManager::getClient() {
  return _client;
}
