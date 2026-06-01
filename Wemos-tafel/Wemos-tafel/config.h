#ifndef CONFIG_H
#define CONFIG_H

// WiFi instellingen
#define WIFI_SSID       "Balls"
#define WIFI_PASSWORD   "adminusee"

// Raspberry Pi server instellingen
#define PI_IP_ADRES     "172.16.221.246"
#define MQTT_SERVER     PI_IP_ADRES

// Statisch IP (verwijder commentaar hieronder om te activeren)
// #define GEBRUIK_STATISCH_IP
// #define STATISCH_IP  192,168,164,100
// #define GATEWAY      192,168,164,1
// #define SUBNET       255,255,255,0

#endif
