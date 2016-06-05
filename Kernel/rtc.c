#include <utils.h>
#include <asmlib.h>
#include <rtc.h>
#include <lib.h>

char * timeStr(char * buffer){
	char * backup=buffer;	
	int h,m,s;
	h=rtc(RTC_HOURS);m=rtc(RTC_MINUTES);s=rtc(RTC_SECONDS);
	int i;
	i=intToString(buffer,h);
	buffer+=i; *buffer=':'; buffer++;
	i=intToString(buffer,m);
	buffer+=i; *buffer=':'; buffer++;
	i=intToString(buffer,s);
	buffer=backup;
	return buffer;
}

TIME * time(){
	TIME * ans = malloc(sizeof(TIME));
	ans->sec=(uint8_t)rtc(RTC_SECONDS);
	ans->min=(uint8_t)rtc(RTC_MINUTES);
	ans->hour=(uint8_t)rtc(RTC_HOURS);
	ans->day=(uint8_t)rtc(RTC_MDAY);
	ans->month=(uint8_t)rtc(RTC_MONTH);
	ans->year=(uint32_t)rtc(RTC_YEAR);
	return ans;
}
