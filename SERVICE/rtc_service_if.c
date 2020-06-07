#include "common.h" 
#include "rtc.h" 
#include "rtc_service_if.h" 


/*....
*/
void rtc_time_service(TIMETYPEDEF *time, RTC_COMMAND command)
{
	switch (command) {
		case GET_RTC_TIME:
			get_time(time);
			break;
		case SET_RTC_TIME :
			set_time(time);
			break;
		default :
			break;
	}		
}



