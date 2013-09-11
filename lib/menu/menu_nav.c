/*
 * menu_nav.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */
#include "menu.h"

MENU_LEVEL_ID_DEF MENU_eGetCurrentItemLevel(void)
{
    return PCURRENT_ITEM->eLevel;
}

void MENU_MenuNavigationHandler(MENU_EVENT_DEF eMenuEvent)
{
    UCHAR ucI;

    switch (eMenuEvent)
    {
        case MENU_ACTION_PAUSE:
            PTDMENU->bMenuActive = FALSE;
            break;

        case MENU_ACTION_RESUME:
            PTDMENU->bMenuActive = TRUE;
            break;

        case MENU_ACTION_NEXT:
            if (TRUE==PTDMENU->bEndMarkerSelected)
            {
                // end marker is currently selected, so wrap around menu
                PTDMENU->bEndMarkerSelected = FALSE;
                // search back first entry on the same menu level
                for (ucI=PTDMENU->ucCurrentItem; ucI>0 ; ucI--)
                {
                    // ignore child levels, react only on parent levels
                    if (MENU_eGetCurrentItemLevel() > atdMenuItems[ucI].eLevel)
                    {
                        break;
                    }
                }
                PTDMENU->ucCurrentItem = ucI;
                return;
            }

            // check if current item is not last one
//          if (PTDMENU->ucCurrentItem + 1 < NUMBER_OF_MENU_ITEMS)
            {
                // search next item on the same level
                for (ucI=PTDMENU->ucCurrentItem + 1; ucI<NUMBER_OF_MENU_ITEMS; ucI++)
                {
                    // item with parent (lower) level found, means no more item on this level
                    if (MENU_eGetCurrentItemLevel() > atdMenuItems[ucI].eLevel)
                    {
                        // next item is from parent level , so no more items - go to end marker emulation
                        break;
                    }
                    if (MENU_eGetCurrentItemLevel() == atdMenuItems[ucI].eLevel)
                    {
                        // next item is from the same level, so scroll to it
                        PTDMENU->ucCurrentItem = ucI;
                        return;
                    }
                }
            }


            // emulate one fake item "end marker"
            PTDMENU->bEndMarkerSelected = TRUE;
            break;

        case MENU_ACTION_SELECT:
            if (TRUE==PTDMENU->bEndMarkerSelected)
            {
                PTDMENU->bEndMarkerSelected = FALSE;
                // if top level, deactivate menu
                if (LVL0==MENU_eGetCurrentItemLevel())
                {
                    MENU_Deactivate();
                }
                else
                // find first entry of parent menu level
                {
                    for (ucI=0; ucI<NUMBER_OF_MENU_ITEMS; ucI++)
                    {
                        if (MENU_eGetCurrentItemLevel()-1 == atdMenuItems[ucI].eLevel)
                        {
                            PTDMENU->ucCurrentItem = ucI;
                            break;
                        }
                    }
                }
                break;
            }

            if (PCURRENT_ITEM->eConfirmation==MENU_CONFIRM_ASK)
            {
                LOG("Needs confirm");
                PTDMENU->bConfirmationScreenActive = TRUE;
                PTDMENU->bConfirmationStateIsNo = TRUE;
            }
            else
            {
                LOG("No need to confirm");
                PTDMENU->bConfirmationStateIsNo = FALSE;
                EventPost(MENU_ACTION_CONFIRMED);
            }
            break;

        case MENU_ACTION_CONFIRMED:
            if (PTDMENU->bConfirmationStateIsNo == FALSE)
            {
                LOG ("Do!");
                MENU_vDoFunction(PCURRENT_ITEM->eMenuFnId);
            }
            else
            {
                LOG ("Ignore!");
            }
            break;

        default:
            RESET("MEN unh evnt");
            break;

    }
}
