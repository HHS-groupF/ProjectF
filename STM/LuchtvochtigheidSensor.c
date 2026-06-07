#include "LuchtvochtigheidSensor.h"
#include <stdio.h>

void LuchtvochtigheidSensor_Create(LuchtvochtigheidSensor_t* sensor, I2C_HandleTypeDef* edge_i2c, float initieleGrens) {
    sensor->huidigewaarde = 0.0f;
    sensor->grenswaarde = initieleGrens;
    sensor->hi2c = edge_i2c;
    sensor->metingKlaar = false;
}

void LuchtvochtigheidSensor_StartMeting(LuchtvochtigheidSensor_t* sensor, TIM_HandleTypeDef* htim) {
    uint8_t cmd[2] = {0x2C, 0x06};
    sensor->metingKlaar = false;

    if (HAL_I2C_Master_Transmit(sensor->hi2c, (0x44 << 1), cmd, 2, HAL_MAX_DELAY) == HAL_OK) {
        // Stel hardware timer in op 15ms voor SHT3x en start interrupt
        __HAL_TIM_SET_AUTORELOAD(htim, 15 - 1);
        __HAL_TIM_SET_COUNTER(htim, 0);
        HAL_TIM_Base_Start_IT(htim);
    } else {
        printf("[ERROR] Failed to transmit to SHT3x Humidity module!\r\n");
        sensor->metingKlaar = true;
    }
}

float LuchtvochtigheidSensor_LeesMeting(LuchtvochtigheidSensor_t* sensor) {
    uint8_t data[6];
    if (HAL_I2C_Master_Receive(sensor->hi2c, (0x44 << 1), data, 6, HAL_MAX_DELAY) == HAL_OK) {
        uint16_t raw_hum = (data[3] << 8) | data[4];
        sensor->huidigewaarde = 100.0f * ((float)raw_hum / 65535.0f);
    } else {
        printf("[ERROR] Failed to receive from SHT3x Humidity module!\r\n");
    }
    sensor->metingKlaar = false;
    return sensor->huidigewaarde;
}

bool LuchtvochtigheidSensor_grensOverschreden(LuchtvochtigheidSensor_t* sensor) {
    return sensor->huidigewaarde > sensor->grenswaarde;
}

void LuchtvochtigheidSensor_setGrenswaarde(LuchtvochtigheidSensor_t* sensor, float nieuweWaarde) {
    sensor->grenswaarde = nieuweWaarde;
}
