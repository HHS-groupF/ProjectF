#include "RGBLampen.h"

RGBLampen::RGBLampen(int rPin, int gPin, int bPin) {
  _rPin = rPin;
  _gPin = gPin;
  _bPin = bPin;
}

void RGBLampen::begin() {
  pinMode(_rPin, OUTPUT);
  pinMode(_gPin, OUTPUT);
  pinMode(_bPin, OUTPUT);
  
  veranderKleur(0, 0, 0);
}

 
void RGBLampen::aangaan() {
  veranderKleur(255, 255, 255);
}

void RGBLampen::veranderKleur(int R, int G, int B) {
  analogWrite(_rPin, R);
  analogWrite(_gPin, G);
  analogWrite(_bPin, B);
}