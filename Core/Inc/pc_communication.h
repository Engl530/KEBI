#ifndef PC_COMMUNICATION_H
#define PC_COMMUNICATION_H

//константы
//===========================================================================
//номера команд от ПК
#define COMMAND_CONNECTION              0x19
#define COMMAND_READ_ALL_SETTINGS       0x28
#define COMMAND_WRITE_SETTINGS_STRUCT		0x37
#define COMMAND_RESET_MCU               0x91
//===========================================================================

#define PC_BUF_SIZE     100
//===========================================================================
//переменные
//===========================================================================

//unsigned char buf[PC_BUF_SIZE];       /*буфер для передачи сообщений компьютеру*/
//unsigned char lenth=0;        /*длина посылки в буфере*/
//unsigned char mess_flag=0;    /*флаг индикатор наличия посылки*/


//===========================================================================


//функции
//===========================================================================
unsigned char CheckRxBufWithMW(unsigned char* buf); /*проверяет буфер с кодовым словом. Возвращает 1 если совпало, если нет то 0*/
unsigned char AnswerStartCommunication_19(unsigned char* buf);//формирует ответ после запроса на начало коммуникации, возвращает длину буфера (вместе с црц)
unsigned char CheckPCmessage(unsigned char* buf, unsigned char len);   //проверяет буфер на наличие посылки от комьютера и готовит ответ. Возвращает 0 если поыслка не наша, если надо ответить возвращает длину буфера
unsigned char ReadAndSendAllSettings(unsigned char* buf); //Читает и записывает в буфер все настройки, возвращает размер буфера(вместе с рцр)
unsigned char AnswerRsetMCU(void);// отвечает на команду ресета и выставляет флаг ресета
unsigned char AnswerWriteSettingStruct(unsigned char* buf);   //формирует ответ и записывает пришедшие настройки в еепром
//===========================================================================


#endif  //PC_COMMUNICATION_H