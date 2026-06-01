#include "SensorBeheerder.h"

SensorBeheerder::SensorBeheerder(MqttClient& mqtt) : _mqtt(mqtt) {}

void SensorBeheerder::verwerkBeweging(const std::string& status) {
    std::lock_guard<std::mutex> lock(_mutex); // voorkomt conflict met de timer thread
    if (status == "JA") {
        _timerActief = false; // annuleer de timer, iemand is er weer
        _mqtt.stuurBericht("sensor/rgb/set", KLEUR_WIT); // tijdelijk, later via Qt dashboard
        std::cout << "\r\033[2K[SENSOR] Beweging gedetecteerd, sfeerlicht AAN.\n> " << std::flush;
    }
    else if (status == "NEE") {
        // start de timer, maar zet de lamp nog NIET uit
        _timerActief = true;
        _geenBewegingSinds = std::chrono::steady_clock::now();
        std::cout << "\r\033[2K[SENSOR] Geen beweging, lamp uit over " << BEWEGING_TIMEOUT_SECONDEN / 60 << " minuten.\n> " << std::flush;
    }
}

void SensorBeheerder::tick() {
    std::lock_guard<std::mutex> lock(_mutex); // voorkomt conflict met de MQTT callback
    // geen actieve timer? niets te doen
    if (!_timerActief) return;

    // bereken hoeveel seconden er verstreken zijn sinds het laatste "NEE"
    auto verstreken = std::chrono::steady_clock::now() - _geenBewegingSinds;
    auto seconden = std::chrono::duration_cast<std::chrono::seconds>(verstreken).count();

    // tijd voorbij? lamp uit
    if (seconden >= BEWEGING_TIMEOUT_SECONDEN) {
        _timerActief = false;
        _mqtt.stuurBericht("sensor/rgb/set", "UIT");
        std::cout << "\r\033[2K[SENSOR] " << BEWEGING_TIMEOUT_SECONDEN / 60 << " min geen beweging, sfeerlicht UIT.\n> " << std::flush;
    }
}