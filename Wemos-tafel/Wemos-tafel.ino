#include "config.h"              // Netwerkinstellingen (SSID, wachtwoord, server IP) 
#include "tafel_config_unit1.h"  
#include "NetwerkManager.h"      
#include "Tafel.h"               
#include "Lichtkrant.h"         

NetwerkManager netwerk(WIFI_SSID, WIFI_PASSWORD, MQTT_SERVER); [cite: 1]
Tafel tafels[] = { TAFEL_CONFIG(netwerk) }; // Tafels worden ingeladen vanuit de config [cite: 2]
const int aantalTafels = sizeof(tafels) / sizeof(Tafel); // Automatisch het aantal tafels berekenen [cite: 2, 3]

// --- LICHTKRANT CONFIGURATIE ---
#define MAX_DEVICES 4
#define CS_PIN D8
Lichtkrant lichtkrant(CS_PIN, MAX_DEVICES);


// BERICHTEN ONTVANGEN VAN DE PI
// ==========================================
void callback(char* topic, byte* payload, unsigned int length) {
  String strTopic = String(topic); [cite: 3]
  
  // 1. Check of het een tafel-reset is [cite: 4]
  for (int i = 0; i < aantalTafels; i++) { [cite: 4]
    String verwachteTopic = "tafel/" + String(tafels[i].getId()) + "/reset"; [cite: 4]
    if (strTopic == verwachteTopic) { [cite: 5]
      tafels[i].zetLampUit(); [cite: 5]
    } [cite: 6]
  }

  // 2. Check of het een bericht voor de lichtkrant is
  // (Laat de Raspberry Pi publiceren op het topic "wemos/lichtkrant")
  if (strTopic == "wemos/lichtkrant") {
    String payloadString = "";
    for (unsigned int i = 0; i < length; i++) {
      payloadString += (char)payload[i];
    }
    // Stuur het ontvangen bericht door naar de lichtkrant klasse
    lichtkrant.zetBericht(payloadString);
  }
}

// SETUP & LOOP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000); // Korte pauze voor de console [cite: 6, 7]
  
  Serial.println("\n--- Restaurant Wemos Systeem Start ---");
  
  netwerk.begin(callback); [cite: 7]
  
  // Setup Tafels
  for (int i = 0; i < aantalTafels; i++) { [cite: 8]
    tafels[i].setup(); [cite: 8]
  }
  
  // Setup Lichtkrant
  lichtkrant.setup();
}

void loop() {
  netwerk.loop(); 
  
  for (int i = 0; i < aantalTafels; i++) { [cite: 9]
    tafels[i].update(); [cite: 9]
  }
  
  lichtkrant.update();
}