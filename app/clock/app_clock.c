/*
 * app_clock.c
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#include <stdio.h>

#include <rtc.h>
#include <hal_lcd.h>
#include <events.h>
#include <config.h>
#include <app.h>


#define CLOCK_DEBUG       1

#if (CLOCK_DEBUG)
  #define CLOCK_PRINTF(f,s...)     printf(f, ##s)
  #define CLOCK_PRINTF_P(f,s...)   printf_P(f, ##s)
  #define CLOCK_PRINTF_T_P(f,s...) printf_P(f, ##s)
#else
  #define CLOCK_PRINTF(x,s...)
  #define CLOCK_PRINTF_P(x,s...)
  #define CLOCK_PRINTF_T_P(x,s...)
#endif
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

static APP_CLOCK_DEF    tdAppClock;
#define ptdAppClock     (&(tdAppClock))


static void APP_CLOCK_vInit(void)
{
    ptdAppClock->ucNewHour = ptdLocalTime->tm_hour;

    ptdAppClock->ucNewMin10  = ptdLocalTime->tm_min  / 10;
    ptdAppClock->ucNewMin1   = ptdLocalTime->tm_min  % 10;

    ptdAppClock->ucNewSec10  = ptdLocalTime->tm_sec  / 10;
    ptdAppClock->ucNewSec1   = ptdLocalTime->tm_sec  % 10;

    ptdAppClock->eCurrentEditPos = AC_POS_HOUR;
    lcd_command (      _BV(LCD_DISPLAYMODE)
                     | _BV(LCD_DISPLAYMODE_ON)
                     | _BV(LCD_DISPLAYMODE_CURSOR)
                     | _BV(LCD_DISPLAYMODE_BLINK)
                   );
}

static void APP_CLOCK_vCleanup(void)
{
    lcd_command (      _BV(LCD_DISPLAYMODE)
                     | _BV(LCD_DISPLAYMODE_ON)
                   );
}
/**
 * Increment *pucValue (by adding iStep) and make wrap around within range ucMin and ucMax
 *
 * @param pucValue
 * @param iStep     TRUE - increment up
 * @param ucMin
 * @param ucMax
 */
static void vIncrementWithRange (unsigned char *pucValue, int iStep, unsigned char ucMin, unsigned char ucMax)
{
    iStep += *pucValue;
    CLOCK_PRINTF_P (PSTR("iStep=%d ucMax=%d ucMin=%d\n"), iStep, ucMax, ucMin);
    if (iStep > ucMax)
    {
        CLOCK_PRINTF_P(PSTR("MAX\n"));
        *pucValue = ucMin;
    }
    else
    if (iStep < ucMin)
    {
        CLOCK_PRINTF_P(PSTR("MIN\n"));
        *pucValue = ucMax;
    }
    else
    {
        CLOCK_PRINTF_P(PSTR("OK\n"));
        *pucValue = iStep;
        CLOCK_PRINTF_P(PSTR("result %d\n"), *pucValue);
    }
}


void APP_CLOCK_vShow(void)
{
    CLOCK_PRINTF_P (PSTR("APP_CLOCK_vShow()\n"));
    CLOCK_PRINTF_P (PSTR("edit pos=%d\n"), ptdAppClock->eCurrentEditPos);
    LCD_vClrScr();

    LCD_vPrintf_P (PSTR("Nowy czas:"));
    LCD_vGotoXY(0,1);
    //                   [ 2:23:45]
    //                   0123456789
    LCD_vPrintf_P (PSTR("[%2d:%d%d:%d%d]"),    ptdAppClock->ucNewHour,
                                               ptdAppClock->ucNewMin10,  ptdAppClock->ucNewMin1,
                                               ptdAppClock->ucNewSec10,  ptdAppClock->ucNewSec1 );

    switch (ptdAppClock->eCurrentEditPos)
    {
        case AC_POS_HOUR:
            LCD_vGotoXY(2,1);
            break;

        case AC_POS_MIN_10:
            LCD_vGotoXY(4,1);
            break;

        case AC_POS_MIN_1:
            LCD_vGotoXY(5,1);
            break;

        case AC_POS_SEC_10:
            LCD_vGotoXY(7,1);
            break;

        case AC_POS_SEC_1:
            LCD_vGotoXY(8,1);
            break;

        default:
            break;
    }

}


void APP_CLOCK_vHandleEvent(EVENT_DEF eEvent)
{
    CLOCK_PRINTF_P(PSTR("APP_CLOCK_vHandleEvent(%d)\n"), eEvent);
    int iStep;
    switch (eEvent)
      {
          case APP_REACTIVATED:
          case APP_ACTIVATED:
              //bNeedsBlinking = TRUE;
              APP_CLOCK_vInit();

              break;

          case APP_LOST_CONTROL:
          case APP_KILLED:
              APP_CLOCK_vCleanup();
              break;

          default:
              break;
      }

    switch (eEvent)
    {
        default:
            break;

        case APP_POPUP_SHOWN:
            APP_vReactivatePreviousApp();
            break;

        case MENU_ACTION_SELECT:
            RTC_vSetTime ( ptdAppClock->ucNewHour,
                           BCD2DEC(ptdAppClock->ucNewMin10 << 4 | ptdAppClock->ucNewMin1),
                           BCD2DEC(ptdAppClock->ucNewSec10 << 4 | ptdAppClock->ucNewSec1)
                         );
            APP_vShowPopupMessage("Zegar ustawiony", UI_POS_POPUP_TIME);
            break;

        case MENU_ACTION_RIGHT:
            ptdAppClock->eCurrentEditPos++;
            if (ptdAppClock->eCurrentEditPos == AC_POS_LAST)
            {
                ptdAppClock->eCurrentEditPos = AC_POS_FIRST;
            }
            break;

        case MENU_ACTION_LEFT:
            if (ptdAppClock->eCurrentEditPos > AC_POS_FIRST)
            {
                ptdAppClock->eCurrentEditPos--;
            }
            else
            {
                ptdAppClock->eCurrentEditPos = AC_POS_LAST-1;
            }
            break;

        case MENU_ACTION_DOWN:
        case MENU_ACTION_UP:
            if (MENU_ACTION_DOWN==eEvent)
            {
                iStep = -1;
            }
            else
            if (MENU_ACTION_UP==eEvent)
            {
                iStep = +1;
            }

            switch (ptdAppClock->eCurrentEditPos)
            {
                case AC_POS_HOUR:
                    vIncrementWithRange (&ptdAppClock->ucNewHour,   iStep, 0, 23);
                    break;

                case AC_POS_MIN_10:
                    vIncrementWithRange (&ptdAppClock->ucNewMin10,  iStep, 0, 5);
                    break;
                case AC_POS_MIN_1:
                    vIncrementWithRange (&ptdAppClock->ucNewMin1,   iStep, 0, 9);
                    break;

                case AC_POS_SEC_10:
                    vIncrementWithRange (&ptdAppClock->ucNewSec10,  iStep, 0, 5);
                    break;
                case AC_POS_SEC_1:
                    vIncrementWithRange (&ptdAppClock->ucNewSec1,   iStep, 0, 9);
                    break;

                default:
                    break;
            }
            break;
    }
}
