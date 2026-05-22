#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <mosquittopp.h>
#include <string>
#include <functional>

// Verbindt met de MQTT broker en stuurt/ontvangt berichten
class MqttClient : public mosqpp::mosquittopp {
private:
    const char* _host; // Opgeslagen voor herverbinding
    int _port;
    std::function<void(std::string, std::string)> onMessageCallback; // Functie die elk bericht verwerkt

public:
    MqttClient(const char *id, const char *host, int port);
    ~MqttClient();

    void on_connect(int rc) override;
    void on_disconnect(int rc) override; // Automatisch herverbinden bij verbindingsverlies
    void on_message(const struct mosquitto_message *message) override;

    void stuurBericht(const std::string& topic, const std::string& payload); // Stuur een bericht naar de broker
    void setCallback(std::function<void(std::string, std::string)> cb);      // Stel in welke functie berichten verwerkt
};

#endif
