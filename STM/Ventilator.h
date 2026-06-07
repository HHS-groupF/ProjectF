#ifndef VENTILATOR_H
#define VENTILATOR_H

#include "main.h"
#include <stdbool.h>

// VENTILATOR_Pin en VENTILATOR_GPIO_Port komen uit main.h (PB0 = D3)

typedef struct {
    bool isAan;
    bool wasAan;          // Onthoudt de staat vóór het brandalarm
    bool noodstopActief;  // Blokkeert alles bij brandalarm
    bool handmatigModus;  // Handmatige bediening, negeert RPi
} Ventilator_t;

void Ventilator_Create(Ventilator_t* vent);
void Ventilator_AanGaan(Ventilator_t* vent, int Status);
void Ventilator_UitGaan(Ventilator_t* vent, int Status);
void Ventilator_noodStop(Ventilator_t* vent);
void Ventilator_HerstelStatus(Ventilator_t* vent);
bool Ventilator_getIsAan(Ventilator_t* vent);

#endif // VENTILATOR_H
