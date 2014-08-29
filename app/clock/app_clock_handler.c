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
#include <texts.h>
#include <tools.h>



APP_CLOCK_DEF    tdAppClock;


static void APP_CLOCK_vInit(void)
{
    ptdAppClock->ucNewHour = RTC_ptdLocalTime->tm_hour;

    ptdAppClock->ucNewMin10  = RTC_ptdLocalTime->tm_min  / 10;
    ptdAppClock->ucNewMin1   = RTC_ptdLocalTime->tm_min  % 10;

    ptdAppClock->ucNewSec10  = RTC_ptdLocalTime->tm_sec  / 10;
    ptdAppClock->ucNewSec1   = RTC_ptdLocalTime->tm_sec  % 10;

    ptdAppClock->eCurrentEditPos = AC_POS_HOUR;
    LCD_vCursorShow();
}

static void APP_CLOCK_vCleanup(void)
{
    LCD_vCursorHide();
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
            EventPost(SYS_RTC_OFFSET_CALC_START); // recalculate time offset
            APP_vShowPopupMessage_P(PSTR_TXT_CLOCK_SET, UI_POS_POPUP_TIME);
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
