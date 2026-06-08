/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin              GPIO_PIN_0
#define MCO_GPIO_Port        GPIOA
#define DRUKKNOP_Pin         GPIO_PIN_1
#define DRUKKNOP_GPIO_Port   GPIOA
#define VCP_TX_Pin           GPIO_PIN_2
#define VCP_TX_GPIO_Port     GPIOA
#define VENTILATOR_Pin       GPIO_PIN_0   // D3 op STM32L432KC = PB0
#define VENTILATOR_GPIO_Port GPIOB
#define SWDIO_Pin            GPIO_PIN_13
#define SWDIO_GPIO_Port      GPIOA
#define SWCLK_Pin            GPIO_PIN_14
#define SWCLK_GPIO_Port      GPIOA
#define VCP_RX_Pin           GPIO_PIN_15
#define VCP_RX_GPIO_Port     GPIOA
#define LD3_Pin              GPIO_PIN_3
#define LD3_GPIO_Port        GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
