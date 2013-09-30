/*
 * rtc.h
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#ifndef RTC_H_
#define RTC_H_

#include <types.h>

/*
 *
    tm_sec: The number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds.
    tm_min: The number of minutes after the hour, in the range 0 to 59.
    tm_hour: The number of hours past midnight, in the range 0 to 23.
    tm_mday: The day of the month, in the range 1 to 31.
    tm_mon: The number of months since January, in the range 0 to 11.
    tm_year: The number of years since 1900.
    tm_wday: The number of days since Sunday, in the range 0 to 6.
    tm_yday: The number of days since January 1, in the range 0 to 365.
    tm_isdst: A flag that indicates whether daylight saving time is in effect at the time described. The value is positive if daylight saving time is in effect, zero if it is not, and negative if the information is not available.
 *
 */
typedef long int time_t;

typedef struct
{
    UCHAR tm_sec;
    UCHAR tm_min;
    UCHAR tm_hour;
    UCHAR tm_mday;
    UCHAR tm_mon;
    UCHAR tm_year;
    UCHAR tm_wday;
    UCHAR tm_yday;
    UCHAR tm_isdst;
} tm;

extern time_t tSecondsUntilEpoch;             ///< actual time in seconds until 1970
extern tm tdLocalTime;          ///< actual time in tm structure
#define ptdLocalTime    (&(tdLocalTime))

#define MIN_PER_HOUR    60
#define HOUR_PER_DAY    24
#define SEC_PER_MIN     60
#define SEC_PER_HOUR    ((MIN_PER_HOUR)*(SEC_PER_MIN))



extern void RTC_vInit(void);
extern void RTC_vConvertLocalTime(void);
extern void RTC_vTickLocalTime(void);
extern void RTC_vShowTime(void);

#endif /* RTC_H_ */
