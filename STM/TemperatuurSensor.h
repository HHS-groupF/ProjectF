#ifndef TEMPERAURE_SENSOR_H_
#define TEMPERAURE_SENSOR_H_

#include "main.h"
#include <stdbool.h>

typedef struct {
    float huidigewaarde;
    float grenswaarde;
    I2C_HandleTypeDef* hi2c;
    volatile bool metingKlaar;
} TemperatuurSensor_t;

void TemperatuurSensor_Create(TemperatuurSensor_t* sensor, I2C_HandleTypeDef* edge_i2c, float initieleGrens);
void TemperatuurSensor_StartMeting(TemperatuurSensor_t* sensor, TIM_HandleTypeDef* htim);
float TemperatuurSensor_LeesMeting(TemperatuurSensor_t* sensor);
bool TemperatuurSensor_grensOverschreden(TemperatuurSensor_t* sensor);
void TemperatuurSensor_setGrenswaarde(TemperatuurSensor_t* sensor, float nieuweWaarde);

#endif /* TEMPERAURE_SENSOR_H_ */
