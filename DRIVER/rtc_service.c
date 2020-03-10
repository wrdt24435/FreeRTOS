#include "common.h"
#include "rtc_service.h"
#include "rtc.h"

int rtc_service(SERVICE_PACKET *sp)
{
	TIMETYPEDEF *led_sp = (TIMETYPEDEF *)sp->buf;
	switch(sp->command_id)
	{
		case GET_RTC_TIME:
			get_time(led_sp);
			break;
		case SET_RTC_TIME:
			set_time(led_sp);
			break;
		default :
			break;
	}
	return 0;
}



