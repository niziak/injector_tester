/*
 * app_clock_display.c
 *
 *  Created on: 28 lip 2014
 *      Author: nizinski_w
 */


#include <stdio.h>

#include <rtc.h>
#include <hal_lcd.h>
#include <events.h>
#include <config.h>
#include <app.h>
#include <texts.h>


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
