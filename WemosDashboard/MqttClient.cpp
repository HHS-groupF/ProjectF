#include "MqttClient.h"
#include <iostream>

MqttClient::MqttClient(const char *id, const char *host, int port) : mosqpp::mosquittopp(id), _host(host), _port(port) {
    mosqpp::lib_init();
    connect(host, port, 60); 
}

MqttClient::~MqttClient() {
    disconnect();
    mosqpp::lib_cleanup();
}

void MqttClient::on_connect(int rc) {
    if (rc == 0) {
        subscribe(NULL, "tafel/+/status");
    }
}

void MqttClient::on_disconnect(int rc) {
    
    if (rc != 0) {
        std::cout << "[MQTT] Verbinding verbroken, opnieuw verbinden..." << std::endl;
        reconnect();
    }
}

void MqttClient::on_message(const struct mosquitto_message *message) {
    std::string topic(message->topic);
    std::string payload(static_cast<char*>(message->payload), message->payloadlen);

    if (onMessageCallback) {
        onMessageCallback(topic, payload); 
    }
}

void MqttClient::stuurBericht(const std::string& topic, const std::string& payload) {
    publish(NULL, topic.c_str(), payload.length(), payload.c_str());
}

void MqttClient::setCallback(std::function<void(std::string, std::string)> cb) {
    onMessageCallback = cb;
}
