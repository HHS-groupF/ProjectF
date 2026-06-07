/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (Pure C - CAN Sensor Node met TX & RX)
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "CO2Sensor.h"
#include "LuchtvochtigheidSensor.h"
#include "TemperatuurSensor.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    STATE_IDLE,
    STATE_WAITING_CO2,
    STATE_WAITING_TEMP,
    STATE_WAITING_HUM
} SystemState_t;

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;

CO2Sensor_t co2;
LuchtvochtigheidSensor_t hum;
TemperatuurSensor_t temp;

CAN_RxHeaderTypeDef RxHeader;
uint8_t             RxData[8];

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_CAN1_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_CAN1_Init();

  CO2Sensor_Create(&co2, &hi2c1, 800.0f);
  LuchtvochtigheidSensor_Create(&hum, &hi2c1, 70.0f);
  TemperatuurSensor_Create(&temp, &hi2c1, 26.0f);

  CO2Sensor_Init(&co2);
  printf("\r\n\r\n[INFO] STM32 CAN-Sensor Node opgestart (TX & RX Actief)!\r\n");

  CAN_FilterTypeDef  sFilterConfig;
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
      printf("[ERROR] CAN Filter init gefaald!\r\n");
      Error_Handler();
  }

  if (HAL_CAN_Start(&hcan1) != HAL_OK) {
      printf("[ERROR] Kon CAN-bus niet starten!\r\n");
      Error_Handler();
  } else {
      printf("[INFO] CAN-bus succesvol gestart.\r\n");
  }

  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
      printf("[ERROR] Kon CAN RX Interrupt niet activeren!\r\n");
      Error_Handler();
  } else {
      printf("[INFO] Luisteren naar inkomende CAN berichten op FIFO0...\r\n");
  }

  uint32_t lastSensorCheck = 0;
  const uint32_t sensorInterval = 1000;
  SystemState_t currentState = STATE_IDLE;

  while (1)
  {
      uint32_t currentMillis = HAL_GetTick();

      if ((currentMillis - lastSensorCheck >= sensorInterval) && (currentState == STATE_IDLE)) {
          lastSensorCheck = currentMillis;
          currentState = STATE_WAITING_CO2;
          CO2Sensor_StartMeting(&co2, &htim2);
      }

      if ((currentState == STATE_WAITING_CO2) && co2.metingKlaar) {
          CO2Sensor_LeesMeting(&co2);
          currentState = STATE_WAITING_TEMP;
          TemperatuurSensor_StartMeting(&temp, &htim2);
      }

      if ((currentState == STATE_WAITING_TEMP) && temp.metingKlaar) {
          TemperatuurSensor_LeesMeting(&temp);
          currentState = STATE_WAITING_HUM;
          LuchtvochtigheidSensor_StartMeting(&hum, &htim2);
      }

      if ((currentState == STATE_WAITING_HUM) && hum.metingKlaar) {
          LuchtvochtigheidSensor_LeesMeting(&hum);
          currentState = STATE_IDLE;

          float c = co2.huidigewaarde;
          float t = temp.huidigewaarde;
          float h = hum.huidigewaarde;

          printf("[TX] Verzenden -> CO2: %.1f ppm | Temp: %.1f C | Vocht: %.1f %%\r\n", c, t, h);

          // FIX: Struct met {0} initialiseren, anders crasht de CAN hardware direct door garbage data!
          CAN_TxHeaderTypeDef TxHeader = {0};
          uint32_t TxMailbox;
          uint8_t TxData[4];

          TxHeader.RTR = CAN_RTR_DATA;
          TxHeader.IDE = CAN_ID_STD;
          TxHeader.DLC = 4;
          TxHeader.TransmitGlobalTime = DISABLE;

          if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0) {
              TxHeader.StdId = 0x100;
              memcpy(TxData, &c, 4);
              HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
          }

          if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0) {
              TxHeader.StdId = 0x101;
              memcpy(TxData, &t, 4);
              HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
          }

          if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0) {
              TxHeader.StdId = 0x102;
              memcpy(TxData, &h, 4);
              HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
          }
      }
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
    Error_Handler();
  }

  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40; // <-- Dit is de cruciale wijziging!
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) { // <-- FLASH_LATENCY_4 is hier ook nodig!
    Error_Handler();
  }

  HAL_RCCEx_EnableMSIPLLMode();
}
static void MX_CAN1_Init(void)
{
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00B07CB4;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 31999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 14;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET); // Zorg dat juiste pins gedefinieerd zijn voor LD3/Vent

  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        HAL_TIM_Base_Stop_IT(htim);
        co2.metingKlaar = true;
        temp.metingKlaar = true;
        hum.metingKlaar = true;
    }
}

//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
//    if (hcan->Instance == CAN1) {
//        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
//            // FIX: Printf en trage functies zijn VERWIJDERD uit deze interrupt, anders crasht je hele systeem direct.
//            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
//        }
//    }
//}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
