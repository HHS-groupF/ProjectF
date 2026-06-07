#include "Ventilator.h"
#include <stdio.h>

void Ventilator_Create(Ventilator_t* vent) {
    vent->isAan = false;
    vent->overRule = false;
}

void Ventilator_AanGaan(Ventilator_t* vent, int Status) {
    HAL_GPIO_WritePin(VENTILATOR_GPIO_Port, VENTILATOR_Pin, GPIO_PIN_SET);
    vent->isAan = true;
    printf(" -> [ACTUATOR] Ingebouwde Ventilator LED AAN (Status code: %d)\r\n", Status);
}

void Ventilator_UitGaan(Ventilator_t* vent, int Status) {
    HAL_GPIO_WritePin(VENTILATOR_GPIO_Port, VENTILATOR_Pin, GPIO_PIN_RESET);
    vent->isAan = false;
    printf(" -> [ACTUATOR] Ingebouwde Ventilator LED UIT (Status code: %d)\r\n", Status);
}

void Ventilator_noodStop(Ventilator_t* vent) {
    HAL_GPIO_WritePin(VENTILATOR_GPIO_Port, VENTILATOR_Pin, GPIO_PIN_RESET);
    vent->isAan = false;
    printf(" -> [🚨 SYSTEM] NOODSTOP! Ingebouwde Ventilator emergency shutdown.\r\n");
}

bool Ventilator_getIsAan(Ventilator_t* vent) {
    return vent->isAan;
}
