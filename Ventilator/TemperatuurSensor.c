#include "TemperatuurSensor.h"
#include <stdio.h>

void TemperatuurSensor_Create(TemperatuurSensor_t* sensor, I2C_HandleTypeDef* edge_i2c, float initieleGrens) {
    sensor->huidigewaarde = 0.0f;
    sensor->grenswaarde = initieleGrens;
    sensor->hi2c = edge_i2c;
    sensor->metingKlaar = false;
}

void TemperatuurSensor_StartMeting(TemperatuurSensor_t* sensor, TIM_HandleTypeDef* htim) {
    uint8_t cmd[2] = {0x2C, 0x06};
    sensor->metingKlaar = false;

    if (HAL_I2C_Master_Transmit(sensor->hi2c, (0x44 << 1), cmd, 2, HAL_MAX_DELAY) == HAL_OK) {
        // Stel hardware timer in op 15ms voor SHT3x en start interrupt
        __HAL_TIM_SET_AUTORELOAD(htim, 15 - 1);
        __HAL_TIM_SET_COUNTER(htim, 0);
        HAL_TIM_Base_Start_IT(htim);
    } else {
        printf("[ERROR] Failed to transmit to SHT3x Temp module!\r\n");
        sensor->metingKlaar = true;
    }
}

float TemperatuurSensor_LeesMeting(TemperatuurSensor_t* sensor) {
    uint8_t data[6];
    if (HAL_I2C_Master_Receive(sensor->hi2c, (0x44 << 1), data, 6, HAL_MAX_DELAY) == HAL_OK) {
        uint16_t raw_temp = (data[0] << 8) | data[1];
        sensor->huidigewaarde = -45.0f + (175.0f * ((float)raw_temp / 65535.0f));
    } else {
        printf("[ERROR] Failed to receive from SHT3x Temp module!\r\n");
    }
    sensor->metingKlaar = false;
    return sensor->huidigewaarde;
}

bool TemperatuurSensor_grensOverschreden(TemperatuurSensor_t* sensor) {
    return sensor->huidigewaarde > sensor->grenswaarde;
}

void TemperatuurSensor_setGrenswaarde(TemperatuurSensor_t* sensor, float nieuweWaarde) {
    sensor->grenswaarde = nieuweWaarde;
}
