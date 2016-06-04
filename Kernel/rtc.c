#include <lib.h>

#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS   4

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
