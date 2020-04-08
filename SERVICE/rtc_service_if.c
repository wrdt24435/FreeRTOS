#include "common.h" 
#include "rtc_service.h" 
#include "rtc_service_if.h" 


/*....
*/
void rtc_time_service(RTC_COMMAND command)
{
	TIMETYPEDEF led_sp;
	switch(command)
	{
		case GET_RTC_TIME:
			get_time(&led_sp);
			break;
		case SET_RTC_TIME:
			set_time(&led_sp);
			break;
		default :
			break;
	}
	return 0;
}



