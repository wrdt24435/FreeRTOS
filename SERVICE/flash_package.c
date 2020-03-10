#include "common.h" 
#include "flash_service.h" 
#include "flash_package.h" 


/*....
*/
void flash_rw_service(u32 addr, u8 *buf, u16 len, FLASH_COMMAND cm)
{
	SERVICE_PACKET sp = {0};
	FLASH_SERVICE_PACKET flash_sp;
	flash_sp.buf = buf;
	flash_sp.len = len;
	flash_sp.addr = addr;
	sp.buf = (void *)&flash_sp;
	sp.type = FLASH_SERVICE;
	sp.command_id = cm;
	submit_service(&sp);
}





