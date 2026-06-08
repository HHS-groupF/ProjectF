#ifndef PIRSENSOR_H
#define PIRSENSOR_H

#include <Arduino.h>

class PirSensor {
  private:
    int _pin;

  public:
    PirSensor(int sensorPin);
    void begin();
    bool detecteertBeweging();
};

#endif