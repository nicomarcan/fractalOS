#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS   4
#define RTC_WDAY    6
#define RTC_MDAY	7
#define RTC_MONTH   8
#define RTC_YEAR 	9

typedef struct{
	uint8_t sec,min,hour,day,month;
	uint32_t year;
} TIME;

char * timeStr(char * buffer);
TIME * time();

#endif
