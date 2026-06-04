#ifndef CANBUSSTM_H
#define CANBUSSTM_H

#include "main.h"
#include "Ventilator.h"
#include <stdbool.h>

// Initialiseert filters, start de bus en zet interrupts aan
void CanBusSTM_Init(CAN_HandleTypeDef* hcan);

// Behandelt het verzenden van de 3 sensorwaarden
void CanBusSTM_SendSensorData(CAN_HandleTypeDef* hcan, float co2, float temp, float hum);

// Controleert in de main-loop of de Raspberry Pi vlaggen heeft gezet
void CanBusSTM_ProcessRx(Ventilator_t* vent);

#endif // CANBUSSTM_H
