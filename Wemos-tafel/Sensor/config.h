#ifndef CONFIG_H
#define CONFIG_H

// WiFi instellingen
#define WIFI_SSID       "Balls"
#define WIFI_PASSWORD   "adminusee"

// Raspberry Pi server (Heimdall / Bifrost) instellingen
#define PI_IP_ADRES     "172.16.221.246"  // IP van de Pi waar Heimdall draait
#define BIFROST_POORT   9000              // poort waarop Heimdall luistert

// --- Pin Configuraties ---
const int PIR_PIN = D1;
const int RED_PIN = D2;
const int GREEN_PIN = D3;
const int BLUE_PIN = D5;


#endif
