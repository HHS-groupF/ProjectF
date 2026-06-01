#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <mosquitto/libmosquittopp.h>
#include <string>
#include <functional>

class MqttClient : public mosqpp::mosquittopp {
private:
    const char* _host; 
    int _port;
    std::function<void(std::string, std::string)> onMessageCallback; 

public:
    MqttClient(const char *id, const char *host, int port);
    ~MqttClient();

    void on_connect(int rc) override;
    void on_disconnect(int rc) override; 
    void on_message(const struct mosquitto_message *message) override;

    void stuurBericht(const std::string& topic, const std::string& payload); 
    void setCallback(std::function<void(std::string, std::string)> cb);      
};

#endif
