#include "config.h"
#include "tafel_config_unit1.h"
#include "SocketWemos.h"
#include "Tafel.h"
#include "Lichtkrant.h"

SocketWemos netwerk(WIFI_SSID, WIFI_PASSWORD, PI_IP_ADRES, BIFROST_POORT);
Tafel tafels[] = {TAFEL_CONFIG(netwerk)};       
const int aantalTafels = sizeof(tafels) / sizeof(Tafel); 

#define MAX_DEVICES 4
#define CS_PIN D8
Lichtkrant lichtkrant(CS_PIN, MAX_DEVICES);


void callback(char *topic, byte *payload, unsigned int length) {
  String strTopic = String(topic);
  
  for (int i = 0; i < aantalTafels; i++) {
     String verwachteTopic = "tafel/" + String(tafels[i].getId()) + "/reset";
     if (strTopic == verwachteTopic) {
       tafels[i].zetLampUit();
     }
  }

  if (strTopic == "wemos/lichtkrant") {
    String payloadString = "";
    for (unsigned int i = 0; i < length; i++) {
      payloadString += (char)payload[i];
    }
    
    if (payloadString.startsWith("MENU:")) {
      lichtkrant.zetStandaardBericht(payloadString.substring(5)); 
    } 
    else if (payloadString.startsWith("MSG:")) {
      lichtkrant.zetTijdelijkBericht(payloadString.substring(4)); 
    }
  }
} 

void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.println("\n--- Start Wemos 1: Tafel & Lichtkrant ---");

  netwerk.begin(callback);

  for (int i = 0; i < aantalTafels; i++) {
    tafels[i].setup();
  }

  lichtkrant.setup();
}

void loop() {
  netwerk.loop();

  for (int i = 0; i < aantalTafels; i++) {
    tafels[i].update();
  }

  lichtkrant.update();
}