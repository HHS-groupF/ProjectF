#include "CO2Sensor.h"
#include <stdio.h>

void CO2Sensor_Create(CO2Sensor_t* sensor, I2C_HandleTypeDef* edge_i2c, float initieleGrens) {
    sensor->huidigewaarde = 400.0f;
    sensor->grenswaarde = initieleGrens;
    sensor->hi2c = edge_i2c;
    sensor->metingKlaar = false;
}

void CO2Sensor_Init(CO2Sensor_t* sensor) {
    uint8_t cmd[2] = {0x20, 0x03};
    if (HAL_I2C_Master_Transmit(sensor->hi2c, (0x58 << 1), cmd, 2, HAL_MAX_DELAY) != HAL_OK) {
        printf("[ERROR] SGP30 CO2-sensor initialisatie mislukt!\r\n");
    } else {
        printf("[SUCCESS] SGP30 CO2-sensor succesvol geïnitialiseerd.\r\n");
    }
}

void CO2Sensor_StartMeting(CO2Sensor_t* sensor, TIM_HandleTypeDef* htim) {
    uint8_t cmd[2] = {0x20, 0x08};
    sensor->metingKlaar = false;

    if (HAL_I2C_Master_Transmit(sensor->hi2c, (0x58 << 1), cmd, 2, HAL_MAX_DELAY) == HAL_OK) {
    	__HAL_TIM_SET_AUTORELOAD(htim, 50 - 1);
        __HAL_TIM_SET_COUNTER(htim, 0);
        HAL_TIM_Base_Start_IT(htim);
    } else {
        printf("[ERROR] Communicatiefout (Transmit) met SGP30 CO2-sensor!\r\n");
        sensor->metingKlaar = true;
    }
}

float CO2Sensor_LeesMeting(CO2Sensor_t* sensor) {
    uint8_t data[6];
    if (HAL_I2C_Master_Receive(sensor->hi2c, (0x58 << 1), data, 6, HAL_MAX_DELAY) == HAL_OK) {
        uint16_t raw_co2 = (data[0] << 8) | data[1];
        sensor->huidigewaarde = (float)raw_co2;
    } else {
        printf("[ERROR] Communicatiefout (Receive) met SGP30 CO2-sensor!\r\n");
    }
    sensor->metingKlaar = false;
    return sensor->huidigewaarde;
}

bool CO2Sensor_grensOverschreden(CO2Sensor_t* sensor) {
    return sensor->huidigewaarde > sensor->grenswaarde;
}

void CO2Sensor_setGrenswaarde(CO2Sensor_t* sensor, float nieuweWaarde) {
    sensor->grenswaarde = nieuweWaarde;
}
