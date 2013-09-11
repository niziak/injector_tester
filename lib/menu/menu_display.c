/*
 * menu_display.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */
#include "menu.h"

void MENU_vShow(void)
{
    if (PTDMENU->bMenuActive==FALSE)
        RESET("show not act");

    MENU_DISP_vClrScr();
    //MENU_DISP_vHome();

    if (TRUE==PTDMENU->bConfirmationScreenActive)
    {
        MENU_DISP_vGotoXY(2,0);
        MENU_DISP_vPuts_P("Czy na pewno?");
#define YES_NO_POS 2
        MENU_DISP_vGotoXY(YES_NO_POS,1);
        //              01234567890
        MENU_DISP_vPuts_P(" Tak   Nie");
        //bBlinkState
        if (FALSE==PTDMENU->bConfirmationStateIsNo)
        {
            MENU_DISP_vGotoXY(YES_NO_POS,1);
        }
        else
        {
            MENU_DISP_vGotoXY(YES_NO_POS+6,1);
        }
        if (bBlinkState==TRUE)
        {
            MENU_DISP_vPuts_P(">");
        }
    }
    else
    {
        MENU_DISP_vPrintf ("%d ", MENU_eGetCurrentItemLevel());
        if (PTDMENU->bEndMarkerSelected)
        {
            MENU_DISP_vPuts(MENU_END_MARKER_TEXT);
        }
        else
        {
            MENU_DISP_vPuts (PCURRENT_ITEM->pcLabel);
        }
    }

//  MENU_DISP_vGotoXY(0,1);
//  LCD_vPrintf ("L%d I%d E%d %d %d", MENU_eGetCurrentItemLevel(), PTDMENU->ucCurrentItem, PTDMENU->bEndMarkerSelected, BTN_SET_GETSTATE, BTN_START_GETSTATE);
}
