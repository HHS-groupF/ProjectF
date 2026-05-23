#ifndef VENTILATOR_H_
#define VENTILATOR_H_

#include "main.h"
#include <stdbool.h>

typedef struct {
    bool isAan;
    bool overRule;
} Ventilator_t;

void Ventilator_Create(Ventilator_t* vent);
void Ventilator_AanGaan(Ventilator_t* vent, int Status);
void Ventilator_UitGaan(Ventilator_t* vent, int Status);
void Ventilator_noodStop(Ventilator_t* vent);
bool Ventilator_getIsAan(Ventilator_t* vent);

#endif /* VENTILATOR_H_ */
