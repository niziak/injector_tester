/*
 * menu_action.c
 *
 *  Created on: Sep 10, 2013
 *      Author: nizinski_w
 */
#include "menu.h"

void MENU_vDoFunction(MENU_FN_ID_DEF eFunctionId)
{
    switch (eFunctionId)
    {
        case MENU_FN_CHILD_MENU:
            PTDMENU->ucCurrentItem++;
            break;

        case MENU_FN_REBOOT:
            for (;;);
            break;

        case MENU_FN_SET_1W_ZASOBNIK:
        case MENU_FN_SET_1W_KRAN:
            break;

        default:
            break;
    }
}
