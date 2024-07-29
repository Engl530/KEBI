#ifndef MODBUS_H
#define MODBUS_H

#define BUF_SZ 256 //размер буфера

//коды ошибок модбаса
//=========================================================================
#define ERROR_01 0x01
#define ERROR_02 0x02
#define ERROR_03 0x03
//=========================================================================

//тайминги модбаса (x*0.1 ms)
//=========================================================================
#define ANSWER_DELAY_2400       	63
#define ANSWER_DELAY_4800       	32
#define ANSWER_DELAY_9600       	14
#define ANSWER_DELAY_19200      	6
#define ANSWER_DELAY_CULC_CONST		100000
//=========================================================================

//uart structure
typedef struct {
unsigned char buffer[BUF_SZ];//буфер
unsigned int rxtimer;//этим мы считаем таймоут
unsigned char rxcnt; //количество принятых символов
unsigned char txcnt;//количество переданных символов
unsigned char txlen;//длина посылки на отправку
unsigned char rxgap;//флаг окончания приема
unsigned char delay;//задержка
} Modbus_struct;

void SetDelay(unsigned int def_delay);
void ModbusCheckMsg(void);//функция проверки принятого сообщения
void ModbusCreateMsg01(void);//функция создания ответа на команду 01 (чтение DO)
void ModbusCreateMsg02(void);//функция создания ответа на команду 02 (чтение DI)
void ModbusCreateMsg03(void);//функция создания ответа на команду 03 (чтение AO)
void ModbusCreateMsg04(void);//функция создания ответа на команду 04 (чтение AI)
void ModbusCreateMsg05(void);//функция создания ответа на команду 05 (запись одного DO)
void ModbusCreateMsg06(void);//функция создания ответа на команду 06 (запись одного AO)
void ModbusCreateMsg15(void);//функция создания ответа на команду 15 (запись нескольких DO)
void ModbusCreateMsg16(void);//функция создания ответа на команду 16 (запись нескольких AO)
void ModbusSendMsg(void);//функция отправки сформированного сообщения
void ModbusCreateMsgError(unsigned char ERROR_CODE);//функция отправки ошибки сообщения
void ModbusTaskLoop(void);//функция проверки модбас сообщения в основном цикле

//Эти функции необходимо вызывать в соответствующем прерывании
//=====================================================================================
void ModbusTimInterruptHandler(void);//функция обработки прерывания таймера который использает модбас
void ModbusUartIntRxHandler(void);//функция обработки прерывания приема юарт который использает модбас
void ModbusUartIntTxHandler(void);//функция обработки прерывания передачи юарт который использает модбас 
//=====================================================================================
unsigned int Crc16(unsigned char* pData,unsigned char len);//функция возвращает crc16
//=========================================================================


#endif // MODBUS_H
