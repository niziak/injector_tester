/*
 * app_clock.h
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#ifndef APP_CLOCK_H_
    #define APP_CLOCK_H_

/**
 * Position of currently edited digit
 */
typedef enum
{
    AC_POS_FIRST   = 0,

    AC_POS_HOUR    = 0,
    AC_POS_MIN_10,
    AC_POS_MIN_1,
    AC_POS_SEC_10,
    AC_POS_SEC_1,

    AC_POS_LAST
} APP_CLOCK_EDIT_POS_DEF;


typedef struct
{
    unsigned char ucNewHour;        ///< tens part of hour
    unsigned char ucNewMin10;       ///< tens part of minutes
    unsigned char ucNewMin1;        ///< ones part of minutes
    unsigned char ucNewSec10;       ///< tens part of seconds
    unsigned char ucNewSec1;        ///< ones part of seconds
    APP_CLOCK_EDIT_POS_DEF eCurrentEditPos;
} APP_CLOCK_DEF;


#define CLOCK_DEBUG       0

#if (CLOCK_DEBUG)
  #define CLOCK_PRINTF(f,s...)     printf(f, ##s)
  #define CLOCK_PRINTF_P(f,s...)   printf_P(f, ##s)
  #define CLOCK_PRINTF_T_P(f,s...) printf_P(f, ##s)
#else
  #define CLOCK_PRINTF(x,s...)
  #define CLOCK_PRINTF_P(x,s...)
  #define CLOCK_PRINTF_T_P(x,s...)
#endif

#define ptdAppClock     (&(tdAppClock))
extern APP_CLOCK_DEF    tdAppClock;

extern void APP_CLOCK_vShow(void);
extern void APP_CLOCK_vHandleEvent(EVENT_DEF eEvent);

#endif /* APP_CLOCK_H_ */
