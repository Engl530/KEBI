/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "modbus.h"
#include "settings.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

extern Settings_struct_type Settings_struct;
coil_state_t_m current_coil_state=0;
coil_state_t_m previous_coil_state=0;
extern work_mode_t_m current_work_mode;
extern work_mode_t_m previous_work_mode;
extern control_source_t_m current_control_source;
extern control_source_t_m previous_control_source;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t reset_flag;
uint16_t ADC_data[CONVERT_COUNT_MAX];
uint16_t Vin_voltage=0;
uint16_t Vcoil_voltage=0;
uint16_t Icoil_current=0;
uint8_t btsp_flag=0;
uint8_t cmt_flag=0;
uint8_t busy_flag=0;
uint32_t vt_on_msec=0;
uint8_t cant_change_flag=1;
extern unsigned char Own_ID;
extern unsigned short AO_Registers[];
extern unsigned short DI_Registers[];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	CheckSettingsInFLASH(Settings_struct);
	if(Settings_struct.control_source != BUTTON_CONTROL) 
	{
		current_coil_state=COIL_CLOSE; 
		AO_Registers[INDEX_COIL_STATE]=COIL_CLOSE;
		AO_Registers[INDEX_CONTROL_SOURCE]=Settings_struct.control_source;
		
	}
	SetDelay(Settings_struct.BaudRate);
	Own_ID=Settings_struct.OwnId;
	LL_ADC_StartCalibration(ADC1);
	while	 (LL_ADC_IsCalibrationOnGoing(ADC1) != 0) {}
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_IWDG_Init();
  MX_USART1_UART_Init();
  MX_TIM14_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	LL_USART_EnableIT_RXNE(USART1);
	
	LL_TIM_EnableCounter(TIM14);
	Own_ID = Settings_struct.OwnId;
	LL_ADC_Enable(ADC1);
	LL_TIM_EnableIT_UPDATE(TIM14);
	LL_TIM_EnableIT_UPDATE(TIM16);
	LL_TIM_EnableIT_UPDATE(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM17);
	ChargeBootstrap();
	AO_Registers[INDEX_SERIAL_NUM] = Settings_struct.SerialNumber;
	AO_Registers[INDEX_DEVICE_CODE] = DEVICE_CODE;
	LED1_ON;
	USART1->CR1 &= ~USART_CR1_TCIE;
	USART1->CR1 &= ~USART_CR1_TXEIE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		BtnRead();
		AO_Read();
		BothControlLoop();
		IWDG_Reset();
		ModbusTaskLoop();
		adcSensLoop();
		CurrControl();
		CoilControl();
		VoltageControl();
		FillMbRegs();
		if (reset_flag) {while(1){}}
			
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI14_Enable();

   /* Wait till HSI14 is ready */
  while(LL_RCC_HSI14_IsReady() != 1)
  {

  }
  LL_RCC_HSI14_SetCalibTrimming(16);
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(48000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_HSI14_EnableADCControl();
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
void IWDG_Reset(void)
{
	IWDG->KR = 0x0000AAAA;
}

void adcSensLoop(void)
{
	uint16_t tmp=0;
	ADC1->CHSELR = 1; //выбрал канал ацп 0
	AdcConvert(ADC_data);
	tmp=AdcAverage(ADC_data);
	Icoil_current=A1_KOEF_SENS*Settings_struct.a1_koef_calibrate*tmp+B1_KOEF_SENS+Settings_struct.b1_koef_calibrate;
	ADC1->CHSELR = 2; //выбрал канал ацп 1
	AdcConvert(ADC_data);
	tmp=AdcAverage(ADC_data);
	Vin_voltage=A2_KOEF_SENS*Settings_struct.a2_koef_calibrate*tmp+B2_KOEF_SENS+Settings_struct.b2_koef_calibrate;
	ADC1->CHSELR = 4; //выбрал канал ацп 2
	AdcConvert(ADC_data);
	tmp=AdcAverage(ADC_data);
	Vcoil_voltage=A3_KOEF_SENS*Settings_struct.a3_koef_calibrate*tmp+B3_KOEF_SENS+Settings_struct.b3_koef_calibrate;
	
	LL_ADC_ClearFlag_EOC(ADC1);
	LL_ADC_ClearFlag_EOS(ADC1);
}


void AdcConvert(uint16_t* data)
{
	uint16_t k;
	k=0;
	uint16_t tmp=0;
	tmp = Settings_struct.pass_convert_count+Settings_struct.convert_count;
	while((ADC1->ISR & ADC_ISR_ADRDY)==0){}
	while(k<tmp)
	{
		LL_ADC_REG_StartConversion(ADC1); 	//старт ацп
		while (!LL_ADC_IsActiveFlag_EOC(ADC1))  {} //ждем окончания преобразования
		if(k>=Settings_struct.pass_convert_count) {data[k-Settings_struct.pass_convert_count]=LL_ADC_REG_ReadConversionData12(ADC1);} //чтение полученного измерения
		else{LL_ADC_REG_ReadConversionData12(ADC1);}
		k++;
		//LL_ADC_REG_StopConversion(ADC1);
		
	}
	//k=0;
}


uint16_t AdcAverage(uint16_t* data)
{
	uint16_t ret_val=0;
	for(uint16_t j=0; j<Settings_struct.convert_count; j++)
	{
		ret_val+=data[j];
	}
	return ret_val/Settings_struct.convert_count;
}


void CoilControl(void)
{
	if(cant_change_flag) 
	{ 
		if (previous_coil_state==COIL_OPEN)
		{ 
			cmt_flag=1; 
			ChargeBootstrap(); 
			LED2_OFF; 
			current_coil_state=COIL_CLOSE;
		}
	return;
	}
	if(previous_coil_state!=current_coil_state)
	{
		switch(current_coil_state)
		{
			case COIL_NOT_DEFINE: VtAllOff(); break;
			case COIL_CLOSE: cmt_flag=1; ChargeBootstrap(); LED2_OFF; break;
			case COIL_OPEN: cmt_flag=1; ChargeBootstrap(); LED2_ON; break;
			default: VtAllOff(); break;
		}
		previous_coil_state=current_coil_state;
	}
}

void BtnRead(void)
{
	static uint8_t old_btn_state=1;
	static uint8_t new_btn_state=1;
	if(Settings_struct.control_source == BUTTON_CONTROL) 
	{
		
		new_btn_state = (BTN_Pin & LL_GPIO_ReadInputPort(BTN_GPIO_Port));
		if (old_btn_state != new_btn_state) 
		{
			if(new_btn_state) current_coil_state=COIL_CLOSE;
			else current_coil_state=COIL_OPEN;
			
		}
		old_btn_state=new_btn_state;
	}
	else
	{
		if(current_coil_state==COIL_OPEN) old_btn_state=0;
		else old_btn_state=1;
		return;
	}
}

void ChargeBootstrap(void)
{
	if (busy_flag) return;
	btsp_flag=1;
	busy_flag=1;
	L1_ON;
	L2_ON;
	//vt_on_msec=Settings_struct.bootstrap_charging_time;
	LL_TIM_EnableCounter(TIM17);
}

void FastBtspCharg(void)
{
	L1_ON;
	L2_ON;
	LL_mDelay(5);
	L1_OFF;
	L2_OFF;
	LL_mDelay(1);
}

void VtDirectPolarity(void)
{
	L2_ON;
	H1_ON;
}

void VtReversPolarity(void)
{
	L1_ON;
	H2_ON;
}

void VtAllOff(void)
{
	L1_OFF;
	L2_OFF;
	H1_OFF;
	H2_OFF;
}

void FillMbRegs(void)
{
	//Settings_struct.control_source = AO_Registers[INDEX_CONTROL_SOURCE];
	
	AO_Registers[INDEX_IN_VOLTAGE] = Vin_voltage;
	AO_Registers[INDEX_COIL_VOLTAGE] = Vcoil_voltage;
	if(BTN_Pin & LL_GPIO_ReadInputPort(BTN_GPIO_Port)) DI_Registers[0]=0;
	else DI_Registers[0]=1;
}

void VoltageControl(void)
{
	if(cant_change_flag)
	{
		AO_Registers[INDEX_COIL_STATE] = COIL_FAULT;
		if(LL_TIM_IsEnabledCounter(TIM3)){}
		else LL_TIM_EnableCounter(TIM3);
		if((Vin_voltage>=Settings_struct.in_volt_good) && (Vcoil_voltage>=Settings_struct.coil_volt_good)) 
		{
			cant_change_flag=0;
			AO_Registers[INDEX_COIL_STATE]=previous_coil_state;
		}
	}
	else
	{
		if((Vin_voltage<(Settings_struct.in_volt_good-Settings_struct.volt_treshold)) || (Vcoil_voltage<(Settings_struct.coil_volt_good-Settings_struct.volt_treshold)))
		{
			cant_change_flag=1;
		}
	}
}



void AO_Read(void)
{
	static uint8_t old_reg_state=0;
	static uint8_t new_reg_state=0;
	if((Settings_struct.control_source == BUTTON_CONTROL) && (!cant_change_flag)) AO_Registers[INDEX_COIL_STATE] = previous_coil_state;
	if(Settings_struct.control_source == MODBUS_CONTROL) 
	{
		
		new_reg_state = AO_Registers[INDEX_COIL_STATE];
		if (old_reg_state != new_reg_state) 
		{
			switch(new_reg_state)
			{
				case COIL_CLOSE: current_coil_state=COIL_CLOSE; break;
				case COIL_OPEN: current_coil_state=COIL_OPEN; break;
				default: break;
			}
		}
		old_reg_state=new_reg_state;
	}
	else 
	{
		old_reg_state = current_coil_state;
		return;
	}
}

void BothControlLoop(void)
{
	static uint8_t previous__di_state=0;
  static uint8_t previous__ao_state=0;	
	if(Settings_struct.control_source == BOTH_CONTROL)
	{
		if(previous__di_state != DI_Registers[0])
		{
			previous__di_state = DI_Registers[0];
			previous__ao_state = AO_Registers[INDEX_COIL_STATE];
			if(DI_Registers[0]==0) {current_coil_state=COIL_CLOSE; previous__di_state = 0; AO_Registers[INDEX_COIL_STATE] = COIL_CLOSE;}
			else {current_coil_state=COIL_OPEN; previous__di_state = 1; AO_Registers[INDEX_COIL_STATE] = COIL_OPEN;}
			}
		if(previous__ao_state != AO_Registers[INDEX_COIL_STATE])
		{
			previous__di_state = DI_Registers[0];
			previous__ao_state = AO_Registers[INDEX_COIL_STATE];
			if(AO_Registers[INDEX_COIL_STATE]==COIL_OPEN) {current_coil_state=COIL_OPEN;}
			else {current_coil_state=COIL_CLOSE;}
		}
	}
	else
	{
		return;
	}
}


void CurrControl(void)
{
	static uint16_t times=0;
	if((Icoil_current>=Settings_struct.curr_treshold)	&& (cant_change_flag))
	{	
		times++;
		if(times>=Settings_struct.curr_def_times)
		{
			times=0;
			while(1){}
		}
	}
	else
	{
		if(times) times--;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
