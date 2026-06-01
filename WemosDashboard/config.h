#ifndef CONFIG_H
#define CONFIG_H

#define MQTT_CLIENT_ID  "PiController"
#define MQTT_SERVER     "127.0.0.1"
#define MQTT_PORT       1883

// Hoe lang geen beweging voordat de lamp uitgaat (in seconden)
#define BEWEGING_TIMEOUT_SECONDEN 1200  // 20 minuten

// Kleur presets voor het sfeerlicht (R, G, B — waarde tussen 0 en 255)
#define KLEUR_WIT   255, 255, 255  // helder wit
#define KLEUR_WARM  255, 120, 20   // warm oranje, gezellige sfeer
#define KLEUR_ROOD  255, 0,   0    // rood
#define KLEUR_BLAUW 0,   0,   255  // blauw
#define KLEUR_GROEN 0,   255, 0    // groen

#endif
