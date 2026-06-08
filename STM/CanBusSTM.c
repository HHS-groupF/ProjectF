<<<<<<< HEAD:Ventilator/CanBusSTM.c
#include "CanBusSTM.h"
#include <stdio.h>
#include <string.h>

static CAN_RxHeaderTypeDef RxHeader;
static uint8_t             RxData[8];
static volatile bool       flag_vent_aan = false;
static volatile bool       flag_vent_uit = false;

void CanBusSTM_Init(CAN_HandleTypeDef* hcan) {
    CAN_FilterTypeDef sFilterConfig;

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

    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK) Error_Handler();
    if (HAL_CAN_Start(hcan) != HAL_OK) Error_Handler();
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) Error_Handler();

    printf("[INFO] CanBusSTM succesvol gestart.\r\n");
}

void CanBusSTM_SendSensorData(CAN_HandleTypeDef* hcan, float co2, float temp, float hum) {
    CAN_TxHeaderTypeDef TxHeader = {0};
    uint32_t TxMailbox;
    uint8_t TxData[4];

    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 4;

    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        TxHeader.StdId = 0x100; memcpy(TxData, &co2, 4);
        HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox);
    }
    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        TxHeader.StdId = 0x101; memcpy(TxData, &temp, 4);
        HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox);
    }
    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        TxHeader.StdId = 0x102; memcpy(TxData, &hum, 4);
        HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox);
    }
}

void CanBusSTM_ProcessRx(Ventilator_t* vent) {
    if (flag_vent_aan) {
        flag_vent_aan = false;
        Ventilator_AanGaan(vent, 1);
    }
    if (flag_vent_uit) {
        flag_vent_uit = false;
        Ventilator_UitGaan(vent, 0);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
            if (RxHeader.StdId == 0x200 && RxHeader.DLC >= 1) {
                if (RxData[0] == 0x01) flag_vent_aan = true;
                else if (RxData[0] == 0x00) flag_vent_uit = true;
            }
        }
    }
}
=======
#include "CanBusSTM.h"
#include <stdio.h>
#include <string.h>

static CAN_RxHeaderTypeDef RxHeader;
static uint8_t             RxData[8];
static volatile bool       flag_vent_aan = false;
static volatile bool       flag_vent_uit = false;

void CanBusSTM_Init(CAN_HandleTypeDef* hcan) {
    CAN_FilterTypeDef sFilterConfig;

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

    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK) Error_Handler();
    if (HAL_CAN_Start(hcan) != HAL_OK) Error_Handler();
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) Error_Handler();

    printf("[INFO] CanBusSTM succesvol gestart.\r\n");
}

void CanBusSTM_SendSensorData(CAN_HandleTypeDef* hcan, float co2, float temp, float hum) {
    CAN_TxHeaderTypeDef TxHeader = {0};
    uint32_t TxMailbox;
    uint8_t TxData[4];

    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 4;

    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        TxHeader.StdId = 0x100; memcpy(TxData, &co2, 4);
        HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox);
    }
    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        TxHeader.StdId = 0x101; memcpy(TxData, &temp, 4);
        HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox);
    }
    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        TxHeader.StdId = 0x102; memcpy(TxData, &hum, 4);
        HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox);
    }
}

void CanBusSTM_ProcessRx(Ventilator_t* vent) {
    if (flag_vent_aan) {
        flag_vent_aan = false;
        Ventilator_AanGaan(vent, 1);
    }
    if (flag_vent_uit) {
        flag_vent_uit = false;
        Ventilator_UitGaan(vent, 0);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
            if (RxHeader.StdId == 0x200 && RxHeader.DLC >= 1) {
                if (RxData[0] == 0x01) flag_vent_aan = true;
                else if (RxData[0] == 0x00) flag_vent_uit = true;
            }
        }
    }
}
>>>>>>> origin/Test-Branch-8:STM/CanBusSTM.c
