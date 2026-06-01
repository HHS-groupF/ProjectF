#ifndef CONFIG_H
#define CONFIG_H

#define MQTT_CLIENT_ID  "PiController"
#define MQTT_SERVER     "127.0.0.1"
#define MQTT_PORT       1883

// Hoe lang geen beweging voordat de lamp uitgaat (in seconden)
#define BEWEGING_TIMEOUT_SECONDEN 1200  // 20 minuten

#endif
