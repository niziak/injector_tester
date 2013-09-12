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
time_t  tNow;
time_t  tOffsetFrom1970;

void RTC_vInit(void)
{
    memset (&tdLocalTime,0, sizeof(tdLocalTime));
    tNow = 0;
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
