#ifndef LUCHTVOCHTIGHEIDSENSOR_H_
#define LUCHTVOCHTIGHEIDSENSOR_H_

#include "main.h"
#include <stdbool.h>

typedef struct {
    float huidigewaarde;
    float grenswaarde;
    I2C_HandleTypeDef* hi2c;
    volatile bool metingKlaar;
} LuchtvochtigheidSensor_t;

void LuchtvochtigheidSensor_Create(LuchtvochtigheidSensor_t* sensor, I2C_HandleTypeDef* edge_i2c, float initieleGrens);
void LuchtvochtigheidSensor_StartMeting(LuchtvochtigheidSensor_t* sensor, TIM_HandleTypeDef* htim);
float LuchtvochtigheidSensor_LeesMeting(LuchtvochtigheidSensor_t* sensor);
bool LuchtvochtigheidSensor_grensOverschreden(LuchtvochtigheidSensor_t* sensor);
void LuchtvochtigheidSensor_setGrenswaarde(LuchtvochtigheidSensor_t* sensor, float nieuweWaarde);

#endif /* LUCHTVOCHTIGHEIDSENSOR_H_ */
