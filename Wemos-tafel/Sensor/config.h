#ifndef CONFIG_H
#define CONFIG_H

// WiFi instellingen
#define WIFI_SSID       "Balls"
#define WIFI_PASSWORD   "adminusee"

// Raspberry Pi server instellingen
#define PI_IP_ADRES     "172.16.221.246"
#define MQTT_SERVER     PI_IP_ADRES

// --- Pin Configuraties ---
const int PIR_PIN = D1;
const int RED_PIN = D2;
const int GREEN_PIN = D3;
const int BLUE_PIN = D5;

#endif
