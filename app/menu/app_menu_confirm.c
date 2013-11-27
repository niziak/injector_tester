/*
 * menu_cfm.c
 *
 *  Created on: Sep 10, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include <app_menu.h>

/**
 * Confirmation "Yes / No" screen handler
 *
 * @param eMenuAction
 */
void MENU_ConfirmationScreenHandler(EVENT_DEF eMenuAction)
{
    if (FALSE==PTDMENU->bConfirmationScreenActive)
    {
        RESET_P(PSTR("app mn cfmna"));
    }

    switch (eMenuAction)
    {
        case MENU_ACTION_LEFT:
//               PTDMENU->bConfirmationScreenActive = FALSE;
//               break;
        case MENU_ACTION_RIGHT:
        case MENU_ACTION_UP:
        case MENU_ACTION_DOWN:
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
