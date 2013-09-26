/*
 * menu.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#include <string.h>

#include <app_menu.h>
#include <key.h> //temp
#include <log.h>
#include <app_status.h>

MENU_DEF tdMenu;


void MENU_vInit(void)
{
    bNeedsBlinking = FALSE;
	PTDMENU->bMenuActive = FALSE;
	PTDMENU->bConfirmationScreenActive = FALSE;
}

BOOL MENU_bIsMenuActive(void)
{
	return PTDMENU->bMenuActive;
}



void MENU_Activate(void)
{
    memset (PTDMENU, 0, sizeof(MENU_DEF));
    bNeedsBlinking = TRUE;
    PTDMENU->bMenuActive = TRUE;
    MENU_DISP_vClrScr();
}

void MENU_Deactivate(void)
{
	PTDMENU->bMenuActive = FALSE;
	bNeedsBlinking = FALSE;
	MENU_DISP_vClrScr();
}





void MENU_HandleEvent(EVENT_DEF eMenuEvent)
{
    switch (eMenuEvent)
    {
        case SYS_UI_TIMEOUT:
            MENU_Deactivate();
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



