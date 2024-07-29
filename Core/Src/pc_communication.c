#include "pc_communication.h"
#include "settings.h"
#include "main.h"
extern unsigned int Crc16(unsigned char* pData,unsigned char len);//функция возвращает crc16

extern Settings_struct_type Settings_struct;
uint8_t reset_flag = 0;
extern uint16_t programming_seconds;
extern uint32_t ReadProgramVersion(void);
const unsigned char magic_word[]="BIKEMI_LL_START_PROGRAMMING";      /*приемная посылка для перехода в режим настроек*/


uint16_t ReadSerialNumber(void)
{
	return Settings_struct.SerialNumber;
}

//проверяет буфер с кодовым словом. Возвращает 1 если совпало, если нет то 0
uint8_t CheckRxBufWithMW(uint8_t* buf)
{
  for(uint8_t i=0; i<(sizeof(magic_word)-1); i++)
  {
    if(buf[i]!=magic_word[i]) return 0;
  }
  return 1;
}


uint8_t AnswerStartCommunication_19(uint8_t* buf)
{
  uint16_t tmp=0;
  buf[0]=COMMAND_CONNECTION;
  buf[1]=~COMMAND_CONNECTION;
  tmp=ReadSerialNumber();
  buf[2]=tmp>>8;
  buf[3]=tmp;
//  TIM1->CNTRH=0; TIM1->CNTRL=0x00;
  return 6; /*4 элемента + 2  байта црц*/
  
}

uint8_t CheckPCmessage(uint8_t* buf, uint8_t len)
{
  uint16_t tmp1=0;
  uint16_t tmp2=0;
  uint8_t tmp3=0;
  tmp1=ReadSerialNumber();
  tmp2=Crc16(buf,len-2);
	programming_seconds=0;
  //проверка серийника, если не верно то выходим
  if(tmp1!=((buf[2]<<8)+(buf[3]))) return 0 ;
  //проверка црц, если не верно то выходим
  if(tmp2!=((buf[len-1]<<8)+buf[len-2])) return 0;
  tmp3=~buf[1];
  if(buf[0]!=tmp3) return 0;
  //если пришли сюда значит посылка верна  она нам
  switch(buf[0])
  {
  case COMMAND_CONNECTION:              return AnswerStartCommunication_19(buf); break;
  case COMMAND_READ_ALL_SETTINGS:       return ReadAndSendAllSettings(buf); break;
  case COMMAND_WRITE_SETTINGS_STRUCT:		return AnswerWriteSettingStruct(buf); break;
  case COMMAND_RESET_MCU:               return AnswerRsetMCU(); break;    
  default: return 0; break;
  }
}

uint8_t ReadAndSendAllSettings(uint8_t* buf)
{
  uint8_t* tmp=(uint8_t*)&Settings_struct;
  uint8_t i=4; //начинаем писать в буфер с четвертого байта
  for(uint8_t k=0; k<(sizeof(Settings_struct)-2); k++)
  {
    buf[i++]= *tmp++;
  }

  return i;
}

uint8_t AnswerRsetMCU()
{
  reset_flag=1;
  return 6; //что получили то и возвращаем
}

uint8_t AnswerWriteSettingStruct(uint8_t* buf)
{
	FLASH_Unlock();
	FLASH_Erase_Page(ADRESS_SETTINGS_STRUCT) ;
	
  uint8_t* tmp=(uint8_t*)&Settings_struct;
  uint8_t i=4; //начинаем писать в буфер с четвертого байта
  for(uint8_t k=0; k<sizeof(Settings_struct)-8; k++)
  {
    *tmp++=buf[i++];
  }
	Settings_struct.chkSum = CulcCheckSum(Settings_struct);
	WriteSettingsStructToFLASH(ADRESS_SETTINGS_STRUCT, Settings_struct);
  return i+=2;
	FLASH_Lock();
}



//-----