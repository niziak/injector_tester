/*
 * rtc.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */
#include "config.h"
#include "rtc.h"
#include <string.h>
#include <util/atomic.h>

tm tdLocalTime;
time_t  tSecondsUntilEpoch;
time_t  tOffsetFrom1970;

void RTC_vInit(void)
{
    memset (&tdLocalTime,0, sizeof(tdLocalTime));
    tSecondsUntilEpoch = 0;
}

/**
 * Get system time
 *
 *
 * @return Number of seconds elapsed since 1970
 */
time_t time(void)
{
    return tOffsetFrom1970 + ulSystemTickS;
}

void stime(time_t *newTime)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        tOffsetFrom1970 = (*newTime) - ulSystemTickS;
    }
}


/**
 * Converts seconds until 1970 stored in @ref tNow to time struct @ref stored in @ref tdLocalTime
 */
void RTC_vConvertLocalTime(void)
{
//    ptdLocalTime->tm_hour = tSecondsUntilEpoch / SEC_PER_HOUR;
//    ptdLocalTime->tm_sec  = tSecondsUntilEpoch;
}


void RTC_vTickLocalTime(void)
{
    if (ptdLocalTime->tm_sec++ > SEC_PER_MIN)
    {
        ptdLocalTime->tm_sec = 0;
        if (ptdLocalTime->tm_min++ > MIN_PER_HOUR)
        {
            ptdLocalTime->tm_min = 0;
            if (ptdLocalTime->tm_hour++ > HOUR_PER_DAY)
            {
                ptdLocalTime->tm_hour = 0;
                ptdLocalTime->tm_mday++;
            }
        }
    }
}
