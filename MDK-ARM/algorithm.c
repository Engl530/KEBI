#include "algorithm.h"
#include "main.h"
#include "settings.h"

device_status_t_m current_device_status=0;
device_status_t_m previous_device_status=0;
mosfet_status_t_m current_mosfet_status=0;
mosfet_status_t_m previous_mosfet_status=0;
coil_status_t_m current_coil_status=0;
coil_status_t_m previous_coil_status=0;
btn_status_t_m current_btn_state=0;

extern control_source_t_m current_control_source;

void BtnControl(void)
{
	switch(current_control_source)
	{
		case BUTTON_CONTROL: 
			if(BTN_Pin & LL_GPIO_ReadInputPort(BTN_GPIO_Port)) 
			break;
		
		case MODBUS_CONTROL: return; break;
		case BOTH_CONTROL: break;
	}
}