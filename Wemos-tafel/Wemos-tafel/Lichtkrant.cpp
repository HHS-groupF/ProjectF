#include "Lichtkrant.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

Lichtkrant::Lichtkrant(uint8_t csPin, uint8_t maxDevices) 
  : display(HARDWARE_TYPE, csPin, maxDevices), toonTijdelijk(false), nieuwBericht(false) {
  // Standaard start-teksten
  strcpy(standaardBericht, "Welkom in het restaurant!");
  strcpy(tijdelijkBericht, "");
}

void Lichtkrant::setup() {
  display.begin();
  display.setIntensity(5);
  display.displayClear();
  
  display.displayText(standaardBericht, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void Lichtkrant::update() {
  if (nieuwBericht) {
    nieuwBericht = false;
    if (toonTijdelijk) {
      display.displayText(tijdelijkBericht, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    } else {
      display.displayText(standaardBericht, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
    display.displayReset();
  }


  if (display.displayAnimate()) {
    
    if (toonTijdelijk) {
      toonTijdelijk = false; 
      display.displayText(standaardBericht, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
    
    // Reset het display om de volgende animatie te starten
    display.displayReset();
  }
}

void Lichtkrant::zetStandaardBericht(String bericht) {
  bericht.toCharArray(standaardBericht, 256);
  // Alleen resetten als er niet toevallig net een ALARM of MSG bezig is
  if (!toonTijdelijk) {
    nieuwBericht = true; 
  }
}

void Lichtkrant::zetTijdelijkBericht(String bericht) {
  bericht.toCharArray(tijdelijkBericht, 256);
  toonTijdelijk = true;
  nieuwBericht = true; // Tijdelijk bericht (msg/tafel) breekt ALTIJD in
}