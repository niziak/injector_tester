/*
 * menu.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#include <string.h>

#include <app.h>
#include <key.h> //temp
#include <log.h>

MENU_DEF tdMenu;


void MENU_vInit(void)
{
	PTDMENU->bMenuActive = FALSE;
	PTDMENU->bConfirmationScreenActive = FALSE;
}

BOOL MENU_bIsMenuActive(void)
{
	return PTDMENU->bMenuActive;
}



void MENU_Activate(void)
{
    if (TRUE == PTDMENU->bMenuActive)
    {
        RESET_P(PSTR("mn already activ!"));
    }
    memset (PTDMENU, 0, sizeof(MENU_DEF));
    PTDMENU->bMenuActive = TRUE;
    MENU_DISP_vClrScr();
}

void MENU_Deactivate(void)
{
    if (FALSE==PTDMENU->bMenuActive)
    {
        RESET_P(PSTR("mn already deactiv!"));
    }
	PTDMENU->bMenuActive = FALSE;
	MENU_DISP_vClrScr();
	// inform app handler that we are no longer servicing
    APP_vReactivatePreviousApp();
}





void MENU_HandleEvent(EVENT_DEF eMenuEvent)
{
    switch (eMenuEvent)
    {
        case APP_REACTIVATED:
        case APP_ACTIVATED:     // received from APP hanlder when menu gets focus
            MENU_vInit();
            MENU_Activate();
            DISP_START_BLINK_TIMER
            break;

        case APP_LOST_CONTROL:
        case APP_KILLED:
            PTDMENU->bMenuActive = FALSE;
            DISP_STOP_BLINK_TIMER
            return;
            break;

        default:
            break;
    }

	if (TRUE == PTDMENU->bConfirmationScreenActive)
	{
//	    LOG("Conf Hndl");
		MENU_ConfirmationScreenHandler(eMenuEvent);
	}
	else
	{
//	    LOG("Menu Hndl");
		MENU_MenuNavigationHandler(eMenuEvent);
	}

}



