#ifndef LICHTKRANT_H
#define LICHTKRANT_H

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

class Lichtkrant {
  private:
    MD_Parola display;
    char standaardBericht[256];
    char tijdelijkBericht[256]; 
    bool toonTijdelijk;         
    bool nieuwBericht;          

  public:
    Lichtkrant(uint8_t csPin, uint8_t maxDevices);

    void setup();
    void update();

    void zetStandaardBericht(String bericht);
    void zetTijdelijkBericht(String bericht);
};

#endif