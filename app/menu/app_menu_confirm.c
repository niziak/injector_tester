/*
 * menu_cfm.c
 *
 *  Created on: Sep 10, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include <app_menu.h>

void MENU_ConfirmationScreenHandler(EVENT_DEF eMenuAction)
{
    if (FALSE==PTDMENU->bConfirmationScreenActive)
        RESET("app mn cfmna");

    switch (eMenuAction)
    {
        case MENU_ACTION_NEXT:
            if (FALSE==PTDMENU->bConfirmationStateIsNo)
            {
                PTDMENU->bConfirmationStateIsNo = TRUE;
            }
            else
            {
                PTDMENU->bConfirmationStateIsNo = FALSE;
            }
            break;

        case MENU_ACTION_SELECT:
            PTDMENU->bConfirmationScreenActive = FALSE;
            EventPost(MENU_ACTION_CONFIRMED);
            break;

        default:
            break;
    }
}