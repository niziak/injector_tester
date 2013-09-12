/*
 * menu_action.c
 *
 *  Created on: Sep 10, 2013
 *      Author: nizinski_w
 */
#include "menu.h"
#include <string.h>
#include <lib/1wire_config.h>
#include <lib/nvm.h>

void MENU_vDoFunction(MENU_FN_ID_DEF eFunctionId)
{
    switch (eFunctionId)
    {
        case MENU_FN_CHILD_MENU:
            PTDMENU->ucCurrentItem++;
            break;

        case MENU_FN_FACTORY_DEFAULT:
            NVM_vRestoreFactory();
            for(;;);
            break;

        case MENU_FN_REBOOT:
            for (;;);
            break;

        case MENU_FN_SET_1W_ZASOBNIK:
            memmove (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM));
            NVM_vSaveSettings();
            break;

        case MENU_FN_SET_1W_KRAN:
            memmove (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, atdNewTempSensors[ONEWIRE_KRAN_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM));
            NVM_vSaveSettings();
            break;

        default:
            break;
    }
}
