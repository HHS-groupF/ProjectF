#ifndef LEDDISPLAY_H_
#define LEDDISPLAY_H_

#include "main.h"

typedef struct {
    GPIO_TypeDef* clk_port;
    uint16_t clk_pin;
    GPIO_TypeDef* dio_port;
    uint16_t dio_pin;
} LEDDisplay_HandleTypeDef;

void leddisplay_init(LEDDisplay_HandleTypeDef* hdisplay);
void leddisplay_clear(LEDDisplay_HandleTypeDef* hdisplay);
void leddisplay_toonGetal(LEDDisplay_HandleTypeDef* hdisplay, int getal);
void leddisplay_toonTemperatuur(LEDDisplay_HandleTypeDef* hdisplay, float temp);

#endif /* LEDDISPLAY_H_ */
