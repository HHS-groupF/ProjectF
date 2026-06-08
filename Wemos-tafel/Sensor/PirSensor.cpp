#include "PirSensor.h"

PirSensor::PirSensor(int sensorPin) {
  _pin = sensorPin;
}


void PirSensor::begin() {
  pinMode(_pin, INPUT);
}

bool PirSensor::detecteertBeweging() {
  return digitalRead(_pin) == HIGH;
}