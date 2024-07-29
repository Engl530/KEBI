#ifndef SETTINGS_H
#define SETTINGS_H
//#include "stm32f0xx_ll_system.h"
//значения настроек производителя
//===========================================================================
#define DEFAULT_CONTROL_SOURCE          0       //источник управления 0-входы, 1-модбас, 2-смешанный
#define DEFAULT_OWN_ID									1       //свой ид
#define DEFAULT_BAUDRATE								9600       //своя скорость
#define DEFAULT_SERIAL_NUMBER						0
#define DEVICE_CODE											54
//===========================================================================

//пороговые значения настроек
//===========================================================================
#define CONTROL_SOURCE_MAX_VALUE        2       /*источник управления 0-входы, 1-модбас, 2-смешанный*/
#define CONTROL_SOURCE_MIN_VALUE        0

#define COIL_STATE_MAX_VALUE        2       /*1-закрыт, 2-открыт*/
#define COIL_STATE_MIN_VALUE        1

#define OWN_ID_MAX_VALUE        254       
#define OWN_ID_MIN_VALUE        0

#define BAUDRATE_MAX_VALUE        38400      
#define BAUDRATE_MIN_VALUE        2400

//===========================================================================

//адреса памяти в ЕЕПРОМ для хранения структур
//===========================================================================
#define ADRESS_SETTINGS_STRUCT		(unsigned int)(0x08003f20)
#define A1_KOEF_SENS							(float)1591e-4					//коэффициенты для расчета
#define B1_KOEF_SENS							(float)0
#define A2_KOEF_SENS							(float)4204e-5
#define B2_KOEF_SENS							(float)11.22
#define A3_KOEF_SENS							(float)7331e-5
#define B3_KOEF_SENS							(float)5.521

#define A1_KOEF_CALIBRATE					(float)1					//коэффициенты для калибровки
#define B1_KOEF_CALIBRATE					(float)0
#define A2_KOEF_CALIBRATE					(float)1
#define B2_KOEF_CALIBRATE					(float)0
#define A3_KOEF_CALIBRATE					(float)1
#define B3_KOEF_CALIBRATE					(float)0

#define CONVERT_COUNT				10	//количество измерений ацп
#define PASS_CONVERT_COUNT	2		//количество первых пропускаемых измерений ацп
#define BOOTSTRAP_CHARGING_TIME		5 //значение в милисекундах 
#define VT_ON_TIME				75 //значение в милисекундах 
#define DEADTIME				1 //значение в милисекундах 
#define IN_VOLT_GOOD		80 //значение в 0.1 В 
#define COIL_VOLT_GOOD		160 //значение в 0.1 В 
#define VOLT_TRESHOLD		5 //значение в 0.1 В 
#define CURR_TRESHOLD		50 //значение в мА 
#define CURR_DEF_TIMES		100 //кол-во проходов

#define SW_VERSION								(unsigned int)(1200) //(x000-версия платы, 0x00-версия прошивки, 00xx-если несильные изменения внесены то версия прошивки не меняется)
/* в версии 1200 добавилась возможность изменения аппаратно значимых значений в управлении клапаном*/
//предельные параметры
//===========================================================================
#define CONVERT_COUNT_MAX		50
#define CONVERT_COUNT_MIN		1

#define PASS_CONVERT_COUNT_MAX	10
#define PASS_CONVERT_COUNT_MIN	0

#define BOOTSTRAP_CHARGING_TIME_MAX		20
#define BOOTSTRAP_CHARGING_TIME_MIN		1

#define VT_ON_TIME_MAX		200
#define VT_ON_TIME_MIN		30

#define DEADTIME_MAX			10
#define DEADTIME_MIN			1

#define IN_VOLT_GOOD_MAX		150
#define IN_VOLT_GOOD_MIN		50

#define COIL_VOLT_GOOD_MAX	300
#define COIL_VOLT_GOOD_MIN	60

#define VOLT_TRESHOLD_MAX		10
#define VOLT_TRESHOLD_MIN		1

#define CURR_TRESHOLD_MAX		1000
#define CURR_TRESHOLD_MIN		10

#define CURR_DEF_TIMES_MAX	500
#define CURR_DEF_TIMES_MIN	5
//===========================================================================
//индексы регистров модбас
//===========================================================================
#define INDEX_CONTROL_SOURCE		0
#define INDEX_COIL_STATE				1
#define INDEX_IN_VOLTAGE				2
#define INDEX_COIL_VOLTAGE			3
#define INDEX_SERIAL_NUM   			4
#define INDEX_DEVICE_CODE				5

//===========================================================================

//структуры
//===========================================================================
typedef struct {
  unsigned int control_source;		//источник управления 0-входы, 1-модбас, 2-смешанный
	unsigned int OwnId;						//свой ИД
	unsigned int BaudRate; 				//скорость
	unsigned int DataWidth;					//длина посылки
  unsigned int StopBits;					//стоп биты
  unsigned int Parity;						//паритет
	unsigned int SerialNumber;			//серийный номер
	float a1_koef_calibrate;				// коэффициент А1 - калибровка входного напряжения
	float b1_koef_calibrate;				// коэффициент В1 - калибровка входного напряжения
	float a2_koef_calibrate;				// коэффициент А2 - калибровка напряжения катушки 
	float b2_koef_calibrate;				// коэффициент В2 - калибровка напряжения катушки  
	float a3_koef_calibrate;				// коэффициент А3 - калибровка тока катушки
	float b3_koef_calibrate;				// коэффициент В3 - калибровка тока катушки
	unsigned int convert_count;			//количество измерений ацп
	unsigned int pass_convert_count;			//количество первых пропускаемых измерений ацп
	unsigned int bootstrap_charging_time;	//время зарядки бутстрепных конденсаторов в мс
	unsigned int vt_on_time;							//время на которое включаются транзисторы для коммутации катушки в мс
	unsigned int dead_time;								//мертвое время между выключением нижнего vt и включением верхнего vt
	unsigned int in_volt_good;						//напряжение нормального входного напряжения в 0,1В
	unsigned int coil_volt_good;			//напряжение нормального напряжения катушки в 0,1В
	unsigned int volt_treshold;				//порог перехода в нормальное напряжение в 0,1В
	unsigned int curr_treshold;				//порог тока катушки в мА
	unsigned int curr_def_times;			//количество измерений тока катушки
	unsigned int sw_version;					//версия прошивки
	unsigned int chkSum;							//чек сумма
} Settings_struct_type;


typedef enum {PRESTART_MODE=0, NORMAL_MODE=1, SETTING_MODE=2}   work_mode_t_m;
typedef enum {BUTTON_CONTROL=0, MODBUS_CONTROL=1, BOTH_CONTROL=2}   control_source_t_m;
typedef enum {COIL_NOT_DEFINE=0, COIL_CLOSE=1, COIL_OPEN=2, COIL_FAULT=3}   coil_state_t_m;



//===========================================================================

//функции
//===========================================================================
//flash
void SetManufactureSettings(void);
void FLASH_Lock(void);
void FLASH_Unlock(void);
_Bool flash_ready(void);
_Bool check_EOP(void);
_Bool FLASH_Erase_Page(unsigned int address); 
_Bool FLASH_Program_Word(unsigned int address,unsigned int data);
unsigned int CulcCheckSum(Settings_struct_type strct);
void WriteSettingsStructToFLASH(unsigned int adress, Settings_struct_type strct);
_Bool ReadSettingsStruct(unsigned int adress);
void WriteSettingsStructToRAM(unsigned int adress, Settings_struct_type strct);
void CheckSettingsInFLASH(Settings_struct_type strct);
unsigned char CheckAndWrightParameters(unsigned char index, unsigned short value, unsigned short* reg);
unsigned char CheckMinMaxValues(Settings_struct_type strct);
//===========================================================================

#endif // SETTINGS_H
