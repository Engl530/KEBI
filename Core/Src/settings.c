#include "settings.h"
#include "main.h"
#include "stm32f0xx_ll_usart.h"
#include <string.h>
#include <stdbool.h>

Settings_struct_type Settings_struct;
extern unsigned short AO_Registers[];
extern unsigned short DI_Registers[];

work_mode_t_m current_work_mode=0;
work_mode_t_m previous_work_mode=0;
control_source_t_m current_control_source=0;
control_source_t_m previous_control_source=0;

//===========================================================================
// записываем в структуру настройки производителя
void SetManufactureSettings(void)
{
  Settings_struct.control_source=DEFAULT_CONTROL_SOURCE;
	Settings_struct.OwnId = DEFAULT_OWN_ID;						/*свой ИД*/  
	Settings_struct.BaudRate = DEFAULT_BAUDRATE; 				/*скорость*/
	Settings_struct.DataWidth = LL_USART_DATAWIDTH_9B;					// длина посылки 0x00001000 - LL_USART_DATAWIDTH_9B, 0 - LL_USART_DATAWIDTH_8B
  Settings_struct.StopBits = LL_USART_STOPBITS_1;					//стоп биты 0 - LL_USART_STOPBITS_1, 0x00002000 - LL_USART_STOPBITS_2
  Settings_struct.Parity = LL_USART_PARITY_EVEN;						// 0x00000400 - LL_USART_PARITY_EVEN , 0 - LL_USART_PARITY_NONE, 0x00000600 - LL_USART_PARITY_ODD, аритет
	Settings_struct.SerialNumber = DEFAULT_SERIAL_NUMBER;
	Settings_struct.a1_koef_calibrate = A1_KOEF_CALIBRATE;
	Settings_struct.b1_koef_calibrate = B1_KOEF_CALIBRATE;
	Settings_struct.a2_koef_calibrate = A2_KOEF_CALIBRATE;
	Settings_struct.b2_koef_calibrate = B2_KOEF_CALIBRATE;
	Settings_struct.a3_koef_calibrate = A3_KOEF_CALIBRATE;
	Settings_struct.b3_koef_calibrate = B3_KOEF_CALIBRATE;
	Settings_struct.convert_count = CONVERT_COUNT;
	Settings_struct.pass_convert_count = PASS_CONVERT_COUNT;
	Settings_struct.bootstrap_charging_time = BOOTSTRAP_CHARGING_TIME;
	Settings_struct.vt_on_time = VT_ON_TIME;
	Settings_struct.dead_time = DEADTIME;
	Settings_struct.in_volt_good = IN_VOLT_GOOD;
	Settings_struct.coil_volt_good = COIL_VOLT_GOOD;
	Settings_struct.volt_treshold = VOLT_TRESHOLD;
	Settings_struct.curr_treshold = CURR_TRESHOLD;
	Settings_struct.curr_def_times = CURR_DEF_TIMES;
	Settings_struct.sw_version = SW_VERSION;
	Settings_struct.chkSum = CulcCheckSum(Settings_struct);
}
//===========================================================================
unsigned int CulcCheckSum(Settings_struct_type strct)
{
	return (strct.control_source 
		+ strct.OwnId 
		+ strct.BaudRate 
		+ strct.DataWidth 
		+ strct.StopBits 
		+ strct.Parity 
		+ strct.SerialNumber 
		+ (unsigned int)strct.a1_koef_calibrate 
		+ (unsigned int)strct.b1_koef_calibrate 
		+ (unsigned int)strct.a2_koef_calibrate 
		+ (unsigned int)strct.b2_koef_calibrate 
		+ (unsigned int)strct.a3_koef_calibrate 
		+ (unsigned int)strct.b3_koef_calibrate 
		+ strct.convert_count
		+ strct.pass_convert_count
		+ strct.bootstrap_charging_time
		+ strct.vt_on_time
		+ strct.dead_time
		+ strct.in_volt_good
		+ strct.coil_volt_good
		+ strct.volt_treshold
		+ strct.curr_treshold
		+ strct.curr_def_times
		+ strct.sw_version)
		+ 1;
}
//===========================================================================
_Bool flash_ready(void) 
{
	return !(FLASH->SR & FLASH_SR_BSY);
}
//===========================================================================
_Bool check_EOP(void) 
{
	if(FLASH->SR & FLASH_SR_EOP)
	{	
		FLASH->SR |= FLASH_SR_EOP;
		return 1;
	}	
	return 0;
}
//===========================================================================
_Bool FLASH_Erase_Page(uint32_t address) 
{
	while(!flash_ready()); //Ожидаем готовности флеша к записи
	
	FLASH->CR|= FLASH_CR_PER; //Устанавливаем бит стирания одной страницы
	FLASH->AR = address; // Задаем её адрес
	FLASH->CR|= FLASH_CR_STRT; // Запускаем стирание
	while(!flash_ready());  //Ждем пока страница сотрется
	FLASH->CR &= ~FLASH_CR_PER; //Сбрасываем бит стирания одной страницы
	
	return check_EOP();//операция завершена, очищаем флаг
}
//===========================================================================
_Bool FLASH_Program_Word(uint32_t address,uint32_t data)
{
	while(!flash_ready()); //Ожидаем готовности флеша к записи
	
	FLASH->CR |= FLASH_CR_PG; //Разрешаем программирование флеша
	*(__IO uint16_t*)address = (uint16_t)data; //Пишем младшие 2 байта
	while(!flash_ready());//Ждем завершения операции
	if(!check_EOP())return 0;
	address+=2;//Прибавляем к адресу два байта
	data>>=16;//Сдвигаем данные
	*(__IO uint16_t*)address = (uint16_t)data; //Пишем старшие 2 байта
	while(!flash_ready());//Ждем завершения операции
	FLASH->CR &= ~(FLASH_CR_PG); //Запрещаем программирование флеша
	
	return check_EOP();
}
//===========================================================================
void FLASH_Unlock(void)
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}
//===========================================================================
void FLASH_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}
//===========================================================================
// записываем структуру настрек в еепром
void WriteSettingsStructToFLASH(unsigned int adress,  Settings_struct_type strct)
{
	unsigned int tmp_adr=adress;
	unsigned int *tmp_data_ptr=(unsigned int*)&strct;
  FLASH_Unlock();
	for(unsigned int i=0; i<sizeof(strct); i+=4)
	{
		FLASH_Program_Word(tmp_adr+i,*tmp_data_ptr++);
	}
	FLASH_Lock();
}
//===========================================================================
_Bool ReadSettingsStruct(unsigned int adress)
{
	unsigned int tmp_adress = adress;
	volatile Settings_struct_type tmp_strct;
	
  tmp_strct.control_source = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.OwnId = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.BaudRate = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.DataWidth = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
  tmp_strct.StopBits = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
  tmp_strct.Parity = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.SerialNumber = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.a1_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.b1_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.a2_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.b2_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.a3_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.b3_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.convert_count = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.pass_convert_count = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.bootstrap_charging_time = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.vt_on_time = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.dead_time = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.in_volt_good = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.coil_volt_good = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.volt_treshold = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.curr_treshold = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.curr_def_times = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.sw_version = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	tmp_strct.chkSum = (*(__IO unsigned int*)tmp_adress);
	if(CheckMinMaxValues(tmp_strct)) return 0;
	if(tmp_strct.chkSum == CulcCheckSum(tmp_strct)) return 1;
	else 
		return 0;
}
//===========================================================================
void WriteSettingsStructToRAM(unsigned int adress, Settings_struct_type strct)
{
	unsigned int tmp_adress = adress;
	Settings_struct.control_source = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.OwnId = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.BaudRate = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.DataWidth = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
  Settings_struct.StopBits = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
  Settings_struct.Parity = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.SerialNumber = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.a1_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.b1_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.a2_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.b2_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.a3_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.b3_koef_calibrate = (*(__IO float*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.convert_count = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.pass_convert_count = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.bootstrap_charging_time = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.vt_on_time = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.dead_time = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.in_volt_good = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.coil_volt_good = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.volt_treshold = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.curr_treshold = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.curr_def_times = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.sw_version = (*(__IO unsigned int*)tmp_adress);
	tmp_adress+=4;
	Settings_struct.chkSum = CulcCheckSum(Settings_struct);
}
//===========================================================================
void CheckSettingsInFLASH(Settings_struct_type strct)
{
	if(ReadSettingsStruct(ADRESS_SETTINGS_STRUCT)) 
	{
		WriteSettingsStructToRAM(ADRESS_SETTINGS_STRUCT, strct);
	}
	else 
	{
		SetManufactureSettings();
		FLASH_Unlock();
		FLASH_Erase_Page(ADRESS_SETTINGS_STRUCT);
		FLASH_Lock();
		WriteSettingsStructToFLASH(ADRESS_SETTINGS_STRUCT, Settings_struct);
	}
	AO_Registers[INDEX_COIL_STATE]=Settings_struct.control_source;
}
//===========================================================================
unsigned char CheckMinMaxValues(Settings_struct_type strct)
{
	if(strct.convert_count<CONVERT_COUNT_MIN || strct.convert_count>CONVERT_COUNT_MAX) return 1;
	if(strct.pass_convert_count<PASS_CONVERT_COUNT_MIN || strct.pass_convert_count>PASS_CONVERT_COUNT_MAX) return 2;
	if(strct.bootstrap_charging_time<BOOTSTRAP_CHARGING_TIME_MIN || strct.bootstrap_charging_time>BOOTSTRAP_CHARGING_TIME_MAX) return 3;
	if(strct.vt_on_time<VT_ON_TIME_MIN || strct.vt_on_time>VT_ON_TIME_MAX) return 4;
	if(strct.dead_time<DEADTIME_MIN || strct.dead_time>DEADTIME_MAX) return 5;
	if(strct.in_volt_good<IN_VOLT_GOOD_MIN || strct.in_volt_good>IN_VOLT_GOOD_MAX) return 6;
	if(strct.coil_volt_good<COIL_VOLT_GOOD_MIN || strct.coil_volt_good>COIL_VOLT_GOOD_MAX) return 7;
	if(strct.volt_treshold<VOLT_TRESHOLD_MIN || strct.volt_treshold>VOLT_TRESHOLD_MAX) return 8;
	if(strct.curr_treshold<CURR_TRESHOLD_MIN || strct.curr_treshold>CURR_TRESHOLD_MAX) return 9;
	if(strct.curr_def_times<CURR_DEF_TIMES_MIN || strct.curr_def_times>CURR_DEF_TIMES_MAX) return 10;
	return 0;
}
//===========================================================================
uint8_t CheckAndWrightParameters(uint8_t index, uint16_t value, unsigned short* reg)
{
  switch (index)
  {
  case INDEX_CONTROL_SOURCE:    if(value>=CONTROL_SOURCE_MIN_VALUE &&   value<=CONTROL_SOURCE_MAX_VALUE)  {Settings_struct.control_source = value; return 0;}   else return 1; break;
  case INDEX_COIL_STATE:    if(value>=COIL_STATE_MIN_VALUE &&   value<=COIL_STATE_MAX_VALUE)    return 0;          else return 1; break;
  default: return 2;
  }
  return 0;
}
//===========================================================================
//----