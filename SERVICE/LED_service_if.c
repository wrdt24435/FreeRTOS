#include "common.h" 
#include "LED_service_if.h" 



/*	����LED_service
*/

void LED_switch_service(LED_X LED, u32 state)
{
	LED_switch(LED , state);
}





