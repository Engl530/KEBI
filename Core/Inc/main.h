/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_iwdg.h"
#include "stm32f0xx_ll_crs.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

void IWDG_Reset(void);
void adcSensLoop(void);
void CoilControl(void);
void AdcConvert(uint16_t* data);
uint16_t AdcAverage(uint16_t* data);
void ChargeBootstrap(void);
void FastBtspCharg(void);
void VtDirectPolarity(void);
void VtReversPolarity(void);
void VtAllOff(void);
void BtnRead(void);
void StartCommutate(void);
void FillMbRegs(void);
void VoltageControl(void);
void AO_Read(void);
void BothControlLoop(void);
void CurrControl(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin LL_GPIO_PIN_1
#define LED1_GPIO_Port GPIOF
#define LED2_Pin LL_GPIO_PIN_0
#define LED2_GPIO_Port GPIOF
#define H1_Pin LL_GPIO_PIN_3
#define H1_GPIO_Port GPIOA
#define L1_Pin LL_GPIO_PIN_4
#define L1_GPIO_Port GPIOA
#define H2_Pin LL_GPIO_PIN_5
#define H2_GPIO_Port GPIOA
#define L2_Pin LL_GPIO_PIN_6
#define L2_GPIO_Port GPIOA
#define RS_DE_Pin LL_GPIO_PIN_7
#define RS_DE_GPIO_Port GPIOA
#define BTN_Pin LL_GPIO_PIN_1
#define BTN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define USART_TRANSMIT	LL_GPIO_SetOutputPin(RS_DE_GPIO_Port, RS_DE_Pin);
#define USART_RECEIVE 	LL_GPIO_ResetOutputPin(RS_DE_GPIO_Port, RS_DE_Pin);

#define L1_ON			LL_GPIO_SetOutputPin(L1_GPIO_Port, L1_Pin);
#define L1_OFF		LL_GPIO_ResetOutputPin(L1_GPIO_Port, L1_Pin);


#define L2_ON			LL_GPIO_SetOutputPin(L2_GPIO_Port, L2_Pin);
#define L2_OFF		LL_GPIO_ResetOutputPin(L2_GPIO_Port, L2_Pin);

#define H1_ON			LL_GPIO_SetOutputPin(H1_GPIO_Port, H1_Pin);
#define H1_OFF		LL_GPIO_ResetOutputPin(H1_GPIO_Port, H1_Pin);

#define H2_ON			LL_GPIO_SetOutputPin(H2_GPIO_Port, H2_Pin);
#define H2_OFF		LL_GPIO_ResetOutputPin(H2_GPIO_Port, H2_Pin);

#define LED1_ON			LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
#define LED1_OFF		LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
#define LED1_TGL		LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

#define LED2_ON			LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
#define LED2_OFF		LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
#define LED2_TGL		LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
