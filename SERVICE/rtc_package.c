#include "common.h" 
#include "rtc_service.h" 
#include "rtc_package.h" 


/*....
*/
void rtc_time_service(RTC_COMMAND command)
{
	SERVICE_PACKET sp = {0};
	TIMETYPEDEF time;
	sp.buf = (void *)&time;
	sp.type = RTC_SERVICE;
	sp.command_id = (u8)command;
	submit_service(&sp);
}



