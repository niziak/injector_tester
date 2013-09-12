/*
 * menu_display.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */
#include "menu.h"

static void vDrawConfirmationScreen(void)
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

void MENU_vShow(void)
{
    if (PTDMENU->bMenuActive==FALSE)
        RESET("shna");

    MENU_DISP_vClrScr();
    //MENU_DISP_vHome();

    if (TRUE==PTDMENU->bConfirmationScreenActive)
    {
        vDrawConfirmationScreen();
    }
    else
    {
        switch (MENU_eGetCurrentItemLevel())
        {
            case LVL0:
                MENU_DISP_vPuts ("MENU");
                break;

            default:
                MENU_DISP_vPuts (atdMenuItems[MENU_ucGetParentItem(PTDMENU->ucCurrentItem)].pcLabel); // show parent label
                break;

        }

        MENU_DISP_vGotoXY(0,1);
        if (bBlinkState==TRUE)
        {
            MENU_DISP_vPuts_P("> ");
        }
        else
        {
            MENU_DISP_vPuts_P("  ");
        }

        if (PTDMENU->bEndMarkerSelected)
        {
            MENU_DISP_vPuts_P(MENU_END_MARKER_TEXT);
        }
        else
        {
            MENU_DISP_vPuts (PCURRENT_ITEM->pcLabel);
        }
    }

//  MENU_DISP_vGotoXY(0,1);
//  LCD_vPrintf ("L%d I%d E%d %d %d", MENU_eGetCurrentItemLevel(), PTDMENU->ucCurrentItem, PTDMENU->bEndMarkerSelected, BTN_SET_GETSTATE, BTN_START_GETSTATE);
}
