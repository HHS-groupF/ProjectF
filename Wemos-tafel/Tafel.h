#ifndef TAFEL_H
#define TAFEL_H

#include <Arduino.h>
#include "NetwerkManager.h" // Belangrijk: we hebben de manager nodig

class Tafel {
  private:
    int _buttonPin;
    int _ledPin;
    int _id;
    bool _helpNodig;                     // true als de lamp aan staat en hulp onderweg is
    int _laatsteKnopStatus;              // Vorige knopstand, voor debounce vergelijking
    unsigned long _laatsteDebounceTijd;
    unsigned long _debounceDelay = 50;   // Knop moet 50ms stabiel zijn voor registratie
    NetwerkManager& _netwerk; // Referentie naar onze netwerk manager

  public:
    Tafel(int bPin, int lPin, int id, NetwerkManager& netwerkManager);
    void setup();
    void update();
    void zetLampAan();
    void zetLampUit();
    int getId();
};

#endif