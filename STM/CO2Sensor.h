#ifndef CO2_SENSOR_H_
#define CO2_SENSOR_H_

#include "main.h"
#include <stdbool.h>

typedef struct {
    float huidigewaarde;
    float grenswaarde;
    I2C_HandleTypeDef* hi2c;
    volatile bool metingKlaar;
} CO2Sensor_t;

void CO2Sensor_Create(CO2Sensor_t* sensor, I2C_HandleTypeDef* edge_i2c, float initieleGrens);
void CO2Sensor_Init(CO2Sensor_t* sensor);
void CO2Sensor_StartMeting(CO2Sensor_t* sensor, TIM_HandleTypeDef* htim);
float CO2Sensor_LeesMeting(CO2Sensor_t* sensor);
bool CO2Sensor_grensOverschreden(CO2Sensor_t* sensor);
void CO2Sensor_setGrenswaarde(CO2Sensor_t* sensor, float nieuweWaarde);

#endif /* CO2_SENSOR_H_ */
