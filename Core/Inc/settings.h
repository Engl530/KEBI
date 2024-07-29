#ifndef SETTINGS_H
#define SETTINGS_H
//#include "stm32f0xx_ll_system.h"
//�������� �������� �������������
//===========================================================================
#define DEFAULT_CONTROL_SOURCE          0       //�������� ���������� 0-�����, 1-������, 2-���������
#define DEFAULT_OWN_ID									1       //���� ��
#define DEFAULT_BAUDRATE								9600       //���� ��������
#define DEFAULT_SERIAL_NUMBER						0
#define DEVICE_CODE											54
//===========================================================================

//��������� �������� ��������
//===========================================================================
#define CONTROL_SOURCE_MAX_VALUE        2       /*�������� ���������� 0-�����, 1-������, 2-���������*/
#define CONTROL_SOURCE_MIN_VALUE        0

#define COIL_STATE_MAX_VALUE        2       /*1-������, 2-������*/
#define COIL_STATE_MIN_VALUE        1

#define OWN_ID_MAX_VALUE        254       
#define OWN_ID_MIN_VALUE        0

#define BAUDRATE_MAX_VALUE        38400      
#define BAUDRATE_MIN_VALUE        2400

//===========================================================================

//������ ������ � ������ ��� �������� ��������
//===========================================================================
#define ADRESS_SETTINGS_STRUCT		(unsigned int)(0x08003f20)
#define A1_KOEF_SENS							(float)1591e-4					//������������ ��� �������
#define B1_KOEF_SENS							(float)0
#define A2_KOEF_SENS							(float)4204e-5
#define B2_KOEF_SENS							(float)11.22
#define A3_KOEF_SENS							(float)7331e-5
#define B3_KOEF_SENS							(float)5.521

#define A1_KOEF_CALIBRATE					(float)1					//������������ ��� ����������
#define B1_KOEF_CALIBRATE					(float)0
#define A2_KOEF_CALIBRATE					(float)1
#define B2_KOEF_CALIBRATE					(float)0
#define A3_KOEF_CALIBRATE					(float)1
#define B3_KOEF_CALIBRATE					(float)0

#define CONVERT_COUNT				10	//���������� ��������� ���
#define PASS_CONVERT_COUNT	2		//���������� ������ ������������ ��������� ���
#define BOOTSTRAP_CHARGING_TIME		5 //�������� � ������������ 
#define VT_ON_TIME				75 //�������� � ������������ 
#define DEADTIME				1 //�������� � ������������ 
#define IN_VOLT_GOOD		80 //�������� � 0.1 � 
#define COIL_VOLT_GOOD		160 //�������� � 0.1 � 
#define VOLT_TRESHOLD		5 //�������� � 0.1 � 
#define CURR_TRESHOLD		50 //�������� � �� 
#define CURR_DEF_TIMES		100 //���-�� ��������

#define SW_VERSION								(unsigned int)(1200) //(x000-������ �����, 0x00-������ ��������, 00xx-���� ��������� ��������� ������� �� ������ �������� �� ��������)
/* � ������ 1200 ���������� ����������� ��������� ��������� �������� �������� � ���������� ��������*/
//���������� ���������
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
//������� ��������� ������
//===========================================================================
#define INDEX_CONTROL_SOURCE		0
#define INDEX_COIL_STATE				1
#define INDEX_IN_VOLTAGE				2
#define INDEX_COIL_VOLTAGE			3
#define INDEX_SERIAL_NUM   			4
#define INDEX_DEVICE_CODE				5

//===========================================================================

//���������
//===========================================================================
typedef struct {
  unsigned int control_source;		//�������� ���������� 0-�����, 1-������, 2-���������
	unsigned int OwnId;						//���� ��
	unsigned int BaudRate; 				//��������
	unsigned int DataWidth;					//����� �������
  unsigned int StopBits;					//���� ����
  unsigned int Parity;						//�������
	unsigned int SerialNumber;			//�������� �����
	float a1_koef_calibrate;				// ����������� �1 - ���������� �������� ����������
	float b1_koef_calibrate;				// ����������� �1 - ���������� �������� ����������
	float a2_koef_calibrate;				// ����������� �2 - ���������� ���������� ������� 
	float b2_koef_calibrate;				// ����������� �2 - ���������� ���������� �������  
	float a3_koef_calibrate;				// ����������� �3 - ���������� ���� �������
	float b3_koef_calibrate;				// ����������� �3 - ���������� ���� �������
	unsigned int convert_count;			//���������� ��������� ���
	unsigned int pass_convert_count;			//���������� ������ ������������ ��������� ���
	unsigned int bootstrap_charging_time;	//����� ������� ����������� ������������� � ��
	unsigned int vt_on_time;							//����� �� ������� ���������� ����������� ��� ���������� ������� � ��
	unsigned int dead_time;								//������� ����� ����� ����������� ������� vt � ���������� �������� vt
	unsigned int in_volt_good;						//���������� ����������� �������� ���������� � 0,1�
	unsigned int coil_volt_good;			//���������� ����������� ���������� ������� � 0,1�
	unsigned int volt_treshold;				//����� �������� � ���������� ���������� � 0,1�
	unsigned int curr_treshold;				//����� ���� ������� � ��
	unsigned int curr_def_times;			//���������� ��������� ���� �������
	unsigned int sw_version;					//������ ��������
	unsigned int chkSum;							//��� �����
} Settings_struct_type;


typedef enum {PRESTART_MODE=0, NORMAL_MODE=1, SETTING_MODE=2}   work_mode_t_m;
typedef enum {BUTTON_CONTROL=0, MODBUS_CONTROL=1, BOTH_CONTROL=2}   control_source_t_m;
typedef enum {COIL_NOT_DEFINE=0, COIL_CLOSE=1, COIL_OPEN=2, COIL_FAULT=3}   coil_state_t_m;



//===========================================================================

//�������
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
