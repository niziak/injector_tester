/*
 * menu_nav.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */
#include <app_menu.h>

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
 * Gets next item from the same menu level as ucCurrentItem
 *
 * @ucCurrentItem   item for which search next item from the same level
 * @return          index in atdMenuItems[] array
 */
UCHAR MENU_ucGetNextItemFromSameLevel(UCHAR ucCurrentItem)
{
    UCHAR ucI;
    for (ucI=ucCurrentItem + 1; ucI<NUMBER_OF_MENU_ITEMS; ucI++)
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
 * Gets previous item from the same menu level as ucCurrentItem
 *
 * @ucCurrentItem   item for which search previous item from the same level
 * @return          index in atdMenuItems[] array
 */
UCHAR MENU_ucGetPrevItemFromSameLevel(UCHAR ucCurrentItem)
{
    UCHAR ucI;
    for (ucI=ucCurrentItem; ucI-- > 0;)
    {
        //DEBUG_P(PSTR("ucI=%d\n"),ucI);
        // item with parent (lower) level found, means no more item on this level
        if (MENU_eGetCurrentItemLevel() > atdMenuItems[ucI].eLevel)
        {
            DEBUG_P(PSTR("ucI=MENU_ITEM_ID_NOT_FOUND\n"));
            // next item is from parent level , so no more items - go to end marker emulation
            return MENU_ITEM_ID_NOT_FOUND;
        }
        if (MENU_eGetCurrentItemLevel() == atdMenuItems[ucI].eLevel)
        {
            DEBUG_P(PSTR("return ucI;\n"));
            // next item is from the same level, so return it
            return ucI;
        }
    }

    DEBUG_P(PSTR("!!MENU_ITEM_ID_NOT_FOUND\n"));
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
    RESET ("AppMnGPIT");
    return MENU_ITEM_ID_NOT_FOUND;
}

/**
 * Get menu item ID for first entry on the current level
 * @return
 */
UCHAR MENU_ucGetFirstItem(void)
{
    if (atdMenuItems[PTDMENU->ucCurrentItem].eLevel==LVL0)
    {
           return MENU_ITEM_ID_FIRST;
    }
    return MENU_ucGetParentItem(PTDMENU->ucCurrentItem)+1;
}

/**
 * Get id of last item from the same menu level (used from wrap around)
 * @return item id
 */
UCHAR MENU_ucGetLastItem(void)
{
    UCHAR ucI;
    for (ucI=MENU_ucGetFirstItem(); ucI<NUMBER_OF_MENU_ITEMS; ucI++)
    {
        if (MENU_ITEM_ID_NOT_FOUND == MENU_ucGetNextItemFromSameLevel(ucI))
        {
            return ucI;
        }
    }
    RESET("MENU_ucGetLastItem");
    return MENU_ITEM_ID_NOT_FOUND;
}

/**
 * Make navigation BACK (for press on end marker or after finished action)
 */
void MENU_vLevelUp(void)
{
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
}

void MENU_MenuNavigationHandler(EVENT_DEF eMenuEvent)
{
    switch (eMenuEvent)
    {
//        case MENU_ACTION_PAUSE:
//            PTDMENU->bMenuActive = FALSE;
//            break;
//
//        case MENU_ACTION_RESUME:
//            PTDMENU->bMenuActive = TRUE;
//            break;

        case MENU_ACTION_UP:
            // if end marker is shown, disable endmarker (automatically last entry will be shown)
            if (TRUE == PTDMENU->bEndMarkerSelected)
            {
                PTDMENU->bEndMarkerSelected = FALSE;
                return;
            }
            // if no previous items left, so make wrap around to last item and show end marker
            if (PTDMENU->ucCurrentItem == MENU_ucGetFirstItem())
            {
                PTDMENU->ucCurrentItem = MENU_ucGetLastItem();
                PTDMENU->bEndMarkerSelected = TRUE;
                return;
            }
//            DEBUG_P(PSTR("MENU_ACTION_UP\n"));

            PTDMENU->ucCurrentItem = MENU_ucGetPrevItemFromSameLevel(PTDMENU->ucCurrentItem);
            if (MENU_ITEM_ID_NOT_FOUND == PTDMENU->ucCurrentItem)
            {
                RESET("AppMnScrNF");
            }
            break;

        case MENU_ACTION_DOWN:
            if (TRUE==PTDMENU->bEndMarkerSelected)
            {
                // end marker is currently selected, so wrap around menu
                PTDMENU->bEndMarkerSelected = FALSE;
                PTDMENU->ucCurrentItem = MENU_ucGetFirstItem();             // search back first entry on the same menu level
                return;
            }


            if (MENU_ITEM_ID_NOT_FOUND == MENU_ucGetNextItemFromSameLevel(PTDMENU->ucCurrentItem))
            {
                // next item is from parent level , so no more items - go to end marker emulation

                // emulate one fake item "end marker"
                PTDMENU->bEndMarkerSelected = TRUE;
            }
            else
            {
                // next item is from the same level, so scroll to it
                PTDMENU->ucCurrentItem = MENU_ucGetNextItemFromSameLevel(PTDMENU->ucCurrentItem);
                return;
            }
            break;

        case MENU_ACTION_SELECT:
            if (TRUE==PTDMENU->bEndMarkerSelected)
            {
                PTDMENU->bEndMarkerSelected = FALSE;
                MENU_vLevelUp();
                break;
            }

            if (PCURRENT_ITEM->eConfirmation==ASK)
            {
                DEBUG_P(PSTR("Cfm ASK\n"));
                PTDMENU->bConfirmationScreenActive = TRUE;
                PTDMENU->bConfirmationStateIsNo = TRUE;
            }
            else
            {
                DEBUG_P(PSTR("Cfm No ASK\n"));
                PTDMENU->bConfirmationStateIsNo = FALSE;
                EventPost(MENU_ACTION_CONFIRMED);
            }
            break;

        case MENU_ACTION_CONFIRMED:
            if (PTDMENU->bConfirmationStateIsNo == FALSE)
            {
                DEBUG_P(PSTR("Confirmed!\n"));
                if (MENU_FN_CHILD_MENU == PCURRENT_ITEM->eMenuFnId)
                {
                    PTDMENU->ucCurrentItem++;
                }
                else
                {
                    MENU_vDoFunction(PCURRENT_ITEM->eMenuFnId);
                    MENU_vLevelUp();
                }
            }
            else
            {
                DEBUG_P (PSTR("Not confiremd - Ignore!\n"));
            }
            break;

        default:
//            LOG("MEN unh evnt");
//            LCD_vPrintf(PSTR(" %d"), eMenuEvent);
            break;

    }
}
