#ifndef PC_COMMUNICATION_H
#define PC_COMMUNICATION_H

//���������
//===========================================================================
//������ ������ �� ��
#define COMMAND_CONNECTION              0x19
#define COMMAND_READ_ALL_SETTINGS       0x28
#define COMMAND_WRITE_SETTINGS_STRUCT		0x37
#define COMMAND_RESET_MCU               0x91
//===========================================================================

#define PC_BUF_SIZE     100
//===========================================================================
//����������
//===========================================================================

//unsigned char buf[PC_BUF_SIZE];       /*����� ��� �������� ��������� ����������*/
//unsigned char lenth=0;        /*����� ������� � ������*/
//unsigned char mess_flag=0;    /*���� ��������� ������� �������*/


//===========================================================================


//�������
//===========================================================================
unsigned char CheckRxBufWithMW(unsigned char* buf); /*��������� ����� � ������� ������. ���������� 1 ���� �������, ���� ��� �� 0*/
unsigned char AnswerStartCommunication_19(unsigned char* buf);//��������� ����� ����� ������� �� ������ ������������, ���������� ����� ������ (������ � ���)
unsigned char CheckPCmessage(unsigned char* buf, unsigned char len);   //��������� ����� �� ������� ������� �� ��������� � ������� �����. ���������� 0 ���� ������� �� ����, ���� ���� �������� ���������� ����� ������
unsigned char ReadAndSendAllSettings(unsigned char* buf); //������ � ���������� � ����� ��� ���������, ���������� ������ ������(������ � ���)
unsigned char AnswerRsetMCU(void);// �������� �� ������� ������ � ���������� ���� ������
unsigned char AnswerWriteSettingStruct(unsigned char* buf);   //��������� ����� � ���������� ��������� ��������� � ������
//===========================================================================


#endif  //PC_COMMUNICATION_H