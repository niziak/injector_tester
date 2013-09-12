/*
 * rtc.h
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#ifndef RTC_H_
#define RTC_H_

#include <types.h>

typedef long int time_t;

typedef struct
{
    UCHAR tm_sec;
    UCHAR tm_min;
    UCHAR tm_hour;
    //UCHAR tm_mday;
    UCHAR tm_mon;
    UCHAR tm_year;
    UCHAR tm_wday;
    UCHAR tm_yday;
    UCHAR tm_isdst;
} tm;

extern time_t tNow;
extern tm tdLocalTime;
#define ptdLocalTime    (&(tdLocalTime))

extern void RTC_vInit(void);

#endif /* RTC_H_ */
