#ifndef CONFIG_H
#define CONFIG_H

// WiFi instellingen
#define WIFI_SSID       "NSELab"
#define WIFI_PASSWORD   "NSELabWiFi"

// Raspberry Pi server (Heimdall / Bifrost) instellingen
#define PI_IP_ADRES     "145.52.127.190"  // IP van de Pi waar Heimdall draait
#define BIFROST_POORT   9000              // poort waarop Heimdall luistert

// Statisch IP (verwijder commentaar hieronder om te activeren)
// #define GEBRUIK_STATISCH_IP
// #define STATISCH_IP  192,168,164,100
// #define GATEWAY      192,168,164,1
// #define SUBNET       255,255,255,0

#endif
