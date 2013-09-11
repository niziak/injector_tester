/*
 * menu_cfm.c
 *
 *  Created on: Sep 10, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include "menu.h"

void MENU_ConfirmationScreenHandler(MENU_EVENT_DEF eMenuAction)
{
    if (FALSE==PTDMENU->bConfirmationScreenActive)
        RESET("123");

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
