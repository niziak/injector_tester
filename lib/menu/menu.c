/*
 * menu.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */


#include <lib/menu/menu.h>
#include <key.h> //temp
#include <log.h>
#include <string.h>

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
    memset (PTDMENU, 0, sizeof(MENU_DEF));
    PTDMENU->bMenuActive = TRUE;
    MENU_DISP_vClrScr();
}

void MENU_Deactivate(void)
{
	PTDMENU->bMenuActive = FALSE;
	MENU_DISP_vClrScr();
}





void MENU_HandleEvent(MENU_EVENT_DEF eMenuAction)
{
    EventClear(); // event is now stored as local variable, don't need events
	if (TRUE==PTDMENU->bConfirmationScreenActive)
	{
	    LOG("Conf Hndl");
		MENU_ConfirmationScreenHandler(eMenuAction);
	}
	else
	{
	    LOG("Menu Hndl");
		MENU_MenuNavigationHandler(eMenuAction);
	}

}



