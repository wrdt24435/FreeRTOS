#include "common.h" 
#include "flash_service.h" 
#include "flash_service_if.h" 


/*....
*/
void flash_rw_service(u32 addr, u8 *buf, u16 len, FLASH_COMMAND command)
{
	switch(command)
	{
		case PERIPHERAL_FLASH_WRITE:
			Flash_Write(buf, addr, len);
			break;
		case PERIPHERAL_FLASH_READ:
			Flash_read(buf, addr, len);
			break;
		default :
			break;
	}
	return 0;
}





