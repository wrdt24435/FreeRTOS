#include "common.h" 
#include "w25q16.h" 
#include "flash_service_if.h" 


/*....
*/
void flash_rw_service(u8* buf,u32 addr,u16 len, FLASH_COMMAND command)
{
	switch (command) {
		case PERIPHERAL_FLASH_READ :
			Flash_read(buf, addr, len);
			break;
		case PERIPHERAL_FLASH_WRITE :
			Flash_Write(buf, addr, len);
			break;
		default :
			break;
	}
}	



