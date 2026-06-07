#ifndef CANBUSS_STM_H
#define CANBUSS_STM_H

#include "stm32l4xx_hal.h"
#include "Ventilator.h"

void CanBusSTM_Init(CAN_HandleTypeDef* hcan);
void CanBusSTM_SendSensorData(CAN_HandleTypeDef* hcan, float co2, float temp, float hum);
void CanBusSTM_ProcessRx(Ventilator_t* vent);
void CanBusSTM_ClearFlags(void);

#endif // CANBUSS_STM_H
