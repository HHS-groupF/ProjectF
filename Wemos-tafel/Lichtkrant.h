#ifndef LICHTKRANT_H
#define LICHTKRANT_H

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

class Lichtkrant {
  private:
    MD_Parola display;
    char huidigBericht[256];  
    bool nieuwBericht;        

  public:
    // Constructor
    Lichtkrant(uint8_t csPin, uint8_t maxDevices);

    // Initialisatie (aanroepen in setup)
    void setup();

    // Verversen van het display (aanroepen in loop)
    void update();

    // Ontvang een nieuw bericht van de Pi via MQTT
    void zetBericht(String bericht);
};

#endif