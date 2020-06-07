#ifndef __RTC_H
#define __RTC_H 		

typedef struct
{
  u8 RTC_Hours;   
  u8 RTC_Minutes;  
  u8 RTC_Seconds;  
  u8 RTC_WeekDay;   
  u8 RTC_Month;  
  u8 RTC_Date;      
  u8 RTC_Year;    
}TIMETYPEDEF;

u8 rtc_init(void);
void get_time(TIMETYPEDEF *time);
void set_time(TIMETYPEDEF *time);



#endif

