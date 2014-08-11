/*
 * app_list_display.c
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



void APP_LIST_vShowDisplay(void)
{
    LIST_PRINTF_P (PSTR("APP_LIST_vShow()\n"));
    LCD_vClrScr();

    // 1st line:
    //     print currently edited mode
    LCD_vPrintf_P (PSTR("Auto%d"), (ptdAppList->eEditedAppMode)-APP_MODE_AUTO_1+1);
    //     print and entry number
    LCD_vPrintf_P (PSTR(" Linia: %d"), ptdAppList->ucCurrentLine + 1);

    // 2nd line:        0123456789012345
    //                  00:00 - 00:00
    LCD_vGotoXY (0,1);
    LCD_vPrintf_P (PSTR("%02d:%02d - %02d:%02d"),
            ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucStartHour,
            ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucStartMin,
            ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucEndHour,
            ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucEndMin      );

    if (ptdAppList->bEditable)
    {
            switch (ptdAppList->eCurrentEditPos)
            {
                case AL_POS_START_HOUR:
                    LCD_vGotoXY(1,1);
                    break;

                case AL_POS_START_MIN_10:
                    LCD_vGotoXY(3,1);
                    break;

                case AL_POS_START_MIN_1:
                    LCD_vGotoXY(4,1);
                    break;

                case AL_POS_END_HOUR:
                    LCD_vGotoXY(9,1);
                    break;

                case AL_POS_END_MIN_10:
                    LCD_vGotoXY(11,1);
                    break;

                case AL_POS_END_MIN_1:
                    LCD_vGotoXY(12,1);
                    break;


                default:
                    break;
            }
        LCD_vCursorShow();
    }
    else
    {
        LCD_vCursorHide();
    }



}
