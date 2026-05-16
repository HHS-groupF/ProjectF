#include "config.h"         // Netwerkinstellingen (SSID, wachtwoord, server IP)
#include "NetwerkManager.h"
#include "Tafel.h"

NetwerkManager netwerk(WIFI_SSID, WIFI_PASSWORD, MQTT_SERVER);

// TAFELS TOEVOEGEN (Knop, Lamp, ID, netwerk)
// ==========================================
Tafel tafels[] = {
  Tafel(D1, D2, 1, netwerk), // Tafel 1
  Tafel(D5, D6, 2, netwerk)  // Tafel 2
};
const int aantalTafels = sizeof(tafels) / sizeof(Tafel);

// BERICHTEN ONTVANGEN VAN DE PI
// ==========================================
void callback(char* topic, byte* payload, unsigned int length) {
  String strTopic = String(topic);
  
  for (int i = 0; i < aantalTafels; i++) {
    String verwachteTopic = "tafel/" + String(tafels[i].getId()) + "/reset";
    if (strTopic == verwachteTopic) {
      tafels[i].zetLampUit();
    }
  }
}

// SETUP & LOOP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000); // Korte pauze voor de console
  
  Serial.println("\n--- Restaurant Wemos Systeem Start ---");
  
  netwerk.begin(callback);
  
  for (int i = 0; i < aantalTafels; i++) {
    tafels[i].setup();
  }
}

void loop() {
  netwerk.loop(); // Houdt WiFi en MQTT in leven
  
  for (int i = 0; i < aantalTafels; i++) {
    tafels[i].update(); // Kijkt of er op een knop is gedrukt
  }
}