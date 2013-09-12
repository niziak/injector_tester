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

#if 0
/**
 * Finds FIRST entry of parent menu level
 * @return index in atdMenuItems[] array
 */
UCHAR MENU_ucGetParentFirstItemId(void)
{
    UCHAR ucI;
    for (ucI=0; ucI<NUMBER_OF_MENU_ITEMS; ucI++)
    {
        if (MENU_eGetCurrentItemLevel()-1 == atdMenuItems[ucI].eLevel)
        {
            return ucI;
        }
    }
    RESET("gpii");
    return MENU_ITEM_ID_NOT_FOUND;
}
#endif

/**
 * Gets next item from the same menu level
 * @return index in atdMenuItems[] array
 */
UCHAR MENU_ucGetNextItem(void)
{
    UCHAR ucI;
    for (ucI=PTDMENU->ucCurrentItem + 1; ucI<NUMBER_OF_MENU_ITEMS; ucI++)
    {
        // item with parent (lower) level found, means no more item on this level
        if (MENU_eGetCurrentItemLevel() > atdMenuItems[ucI].eLevel)
        {
            // next item is from parent level , so no more items - go to end marker emulation
            return MENU_ITEM_ID_NOT_FOUND;
        }
        if (MENU_eGetCurrentItemLevel() == atdMenuItems[ucI].eLevel)
        {
            // next item is from the same level, so return it
            return ucI;
        }
    }
    return MENU_ITEM_ID_NOT_FOUND;
}



/**
 * Finds parent menu item, first child can be calculated by adding one - for wrap around
 * @return index in atdMenuItems[] array
 */
UCHAR MENU_ucGetParentItem(UCHAR ucCurrentItem)
{
    UCHAR ucI;

    if (atdMenuItems[ucCurrentItem].eLevel==LVL0)
    {
        return MENU_ITEM_ID_FIRST;
    }
    // search back first entry on the same menu level
    for (ucI=ucCurrentItem; ucI-- > 0 ; )   // unigned ucI counts to 0
    {
//        LCD_vClrScr();
//        LCD_vPrintf("ucI=%d citem=%d", ucI, ucCurrentItem);
//        LCD_vGotoXY(0,1);
//        LCD_vPrintf("el=%d < clvl=%d", atdMenuItems[ucI].eLevel, MENU_eGetCurrentItemLevel());
//        _delay_ms(1000);
        // ignore child levels, react only on parent levels
        if (atdMenuItems[ucI].eLevel < MENU_eGetCurrentItemLevel())
        {
            // parent entry found
            return ucI;
        }
    }
//    return ucI; // LVL0 wrap around, so back to first menu item
    RESET ("gpit");
    return MENU_ITEM_ID_NOT_FOUND;
}


UCHAR MENU_ucGetFirstItem(void)
{
    if (atdMenuItems[PTDMENU->ucCurrentItem].eLevel==LVL0)
    {
           return MENU_ITEM_ID_FIRST;
    }
    return MENU_ucGetParentItem(PTDMENU->ucCurrentItem)+1;
}


void MENU_MenuNavigationHandler(MENU_EVENT_DEF eMenuEvent)
{
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
                PTDMENU->ucCurrentItem = MENU_ucGetFirstItem();             // search back first entry on the same menu level
                return;
            }


            if (MENU_ITEM_ID_NOT_FOUND == MENU_ucGetNextItem())
            {
                // next item is from parent level , so no more items - go to end marker emulation

                // emulate one fake item "end marker"
                PTDMENU->bEndMarkerSelected = TRUE;
            }
            else
            {
                // next item is from the same level, so scroll to it
                PTDMENU->ucCurrentItem = MENU_ucGetNextItem();
                return;
            }
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
                    PTDMENU->ucCurrentItem = MENU_ucGetParentItem(PTDMENU->ucCurrentItem); //MENU_ucGetParentFirstItemId();
                }
                break;
            }

            if (PCURRENT_ITEM->eConfirmation==ASK)
            {
//                LOG("Cfm ASK");
                PTDMENU->bConfirmationScreenActive = TRUE;
                PTDMENU->bConfirmationStateIsNo = TRUE;
            }
            else
            {
//                LOG("Cfm No ASK");
                PTDMENU->bConfirmationStateIsNo = FALSE;
                EventPost(MENU_ACTION_CONFIRMED);
            }
            break;

        case MENU_ACTION_CONFIRMED:
            if (PTDMENU->bConfirmationStateIsNo == FALSE)
            {
//                LOG ("Do!");
                MENU_vDoFunction(PCURRENT_ITEM->eMenuFnId);
            }
            else
            {
//                LOG ("Ignore!");
            }
            break;

        default:
//            LOG("MEN unh evnt");
//            LCD_vPrintf(PSTR(" %d"), eMenuEvent);
            break;

    }
}
