#include "Lichtkrant.h"

// Definieer de hardware instellingen (zoals in de originele code)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

Lichtkrant::Lichtkrant(uint8_t csPin, uint8_t maxDevices) 
  : display(HARDWARE_TYPE, csPin, maxDevices), nieuwBericht(false) {
  // Standaardbericht tijdens het opstarten
  strcpy(huidigBericht, "Wachten op Pi...");
}

void Lichtkrant::setup() {
  display.begin();
  display.setIntensity(5); // Helderheid 0-15
  display.displayClear();
  
  // Start de eerste animatie
  display.displayText(huidigBericht, PA_CENTER, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void Lichtkrant::update() {
  // displayAnimate() verplaatst de tekst één stapje en geeft 'true' terug als de animatie klaar is
  if (display.displayAnimate()) {
    if (nieuwBericht) {
      // Als er via MQTT een nieuw bericht is ingesteld, reset de animatie
      display.displayReset();
      nieuwBericht = false;
    }
  }
}

void Lichtkrant::zetBericht(String bericht) {
  // Zet de String van de Pi om naar een char array voor MD_Parola
  bericht.toCharArray(huidigBericht, 256);
  nieuwBericht = true;
}