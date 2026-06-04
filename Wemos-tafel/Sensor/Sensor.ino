#include "config.h"
#include "SocketWemos.h"
#include "PirSensor.h"
#include "RGBLampen.h"

SocketWemos netwerk(WIFI_SSID, WIFI_PASSWORD, PI_IP_ADRES, BIFROST_POORT);

PirSensor sensor(PIR_PIN);
RGBLampen rgb(RED_PIN, GREEN_PIN, BLUE_PIN);

bool vorigeBeweging = false;

void callback(char *topic, byte *payload, unsigned int length) {
  String strTopic = String(topic);
  String commando = "";
  for (unsigned int i = 0; i < length; i++) {
    commando += (char)payload[i];
  }

  if (strTopic == "sensor/rgb/set") {
    if (commando == "UIT") {
      // lamp uit
      rgb.veranderKleur(0, 0, 0);
    } else {
      // verwacht formaat: "R,G,B" bv. "255,120,20"
      int r = commando.substring(0, commando.indexOf(',')).toInt();
      commando = commando.substring(commando.indexOf(',') + 1);
      int g = commando.substring(0, commando.indexOf(',')).toInt();
      int b = commando.substring(commando.indexOf(',') + 1).toInt();
      rgb.veranderKleur(r, g, b);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Start Wemos 2 (DUMB NODE): Sfeer & Sensor ---");

  netwerk.begin(callback);
  sensor.begin();
  rgb.begin();
}

void loop() {
  netwerk.loop();

  bool huidigeBeweging = sensor.detecteertBeweging();

  if (huidigeBeweging != vorigeBeweging) {
    if (huidigeBeweging == true) {
      netwerk.stuurBericht("sensor/beweging", "JA");
      Serial.println("Verstuurd naar Pi: Beweging gestart");
    } else {
      netwerk.stuurBericht("sensor/beweging", "NEE");
      Serial.println("Verstuurd naar Pi: Beweging gestopt");
    }
    vorigeBeweging = huidigeBeweging; // Update het geheugen
  }
}
