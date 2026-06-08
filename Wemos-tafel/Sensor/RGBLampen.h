#ifndef RGBLAMPEN_H
#define RGBLAMPEN_H

#include <Arduino.h>

class RGBLampen {
  private:
 
    int _rPin;
    int _gPin;
    int _bPin;

  public:
    RGBLampen(int rPin, int gPin, int bPin);
    
    void begin();
    void aangaan();
    void veranderKleur(int R, int G, int B);
};

#endif