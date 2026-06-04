#ifndef TAFEL_H
#define TAFEL_H

#include <Arduino.h>
#include "SocketWemos.h"

class Tafel {
  private:
    int _buttonPin;
    int _ledPin;
    int _id;
    bool _helpNodig;                     
    int _laatsteKnopStatus;              
    unsigned long _laatsteDebounceTijd;
    unsigned long _debounceDelay = 50;
    SocketWemos& _netwerk;
  public:
    Tafel(int bPin, int lPin, int id, SocketWemos& netwerkManager);
    void setup();
    void update();
    void zetLampAan();
    void zetLampUit();
    int getId();
};

#endif