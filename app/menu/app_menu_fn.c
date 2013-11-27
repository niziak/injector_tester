/*
 * menu_action.c
 *
 *  Created on: Sep 10, 2013
 *      Author: nizinski_w
 */
#include <string.h>


#include <app_menu.h>
#include <1wire_config.h>
#include <lib/nvm.h>
#include <app.h>
#include <1wire.h>

/**
 * Perform action on selected menu item
 *
 * @param eFunctionId
 */
void MENU_vDoFunction(MENU_FN_ID_DEF eFunctionId)
{
    switch (eFunctionId)
    {
        case MENU_FN_SETTINGS_AUTO_2:
        case MENU_FN_SETTINGS_AUTO_3:
        case MENU_FN_SETTINGS_AUTO_4:
            APP_vActivateApp(APP_LIST);
            break;

        case MENU_FN_APP_MODE_24H:
        case MENU_FN_APP_MODE_AUTO_1:
        case MENU_FN_APP_MODE_AUTO_2:
        case MENU_FN_APP_MODE_AUTO_3:
        case MENU_FN_APP_MODE_AUTO_4:
            // Dirty calculation of mode
            eAppMode = APP_MODE_24H /* ZERO */ + ( eFunctionId - MENU_FN_APP_MODE_24H );
            APP_vShowPopupMessage("Zmieniono tryb", UI_POS_POPUP_TIME);
            break;

        case MENU_FN_FACTORY_DEFAULT:
            NVM_vRestoreFactory();
            RESET_P(PSTR("FACTORY RESET!"));
            break;

        case MENU_FN_10MIN:
            if (uiPumpSwitchOffAfter>0)
            {
                bPumpIsRunning = 0;
                uiPumpSwitchOffAfter = 0;
            }
            else
            {
                bPumpIsRunning = 1;
                uiPumpSwitchOffAfter = PUMP_MANUAL_TTL;
            }
            break;

        case MENU_FN_SET_CLOCK:
            APP_vActivateApp(APP_CLOCK);
            break;

        case MENU_FN_REBOOT:
            RESET_P(PSTR("RESTART!"));
            break;

        case MENU_FN_1W_SHOW:
            break;

        case MENU_FN_1W_DETECT:
            OWDetectDevices();
            // 2 sensors are required
            if ( (atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM[0] == 0) || (atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM[0] == 0) )
            {
                APP_vShowPopupMessage_P(PSTR("Brak 2 nowych!"), UI_NEG_POPUP_TIME);
            }
            else
            {
                memmove (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM));
                memmove (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, atdNewTempSensors[ONEWIRE_KRAN_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM));
                NVM_vSaveSettings();
                APP_vShowPopupMessage(PSTR("2 nowe"), UI_POS_POPUP_TIME);
            }
            break;

        case MENU_FN_1W_DELETE:
            memset (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, 0, sizeof(atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM));
            memset (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, 0, sizeof(atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM));
            NVM_vSaveSettings();
            APP_vShowPopupMessage(PSTR("Usunieto"), UI_POS_POPUP_TIME);
            break;

        case MENU_FN_1W_CHANGE:
            {
                UCHAR aucTempROM[OW_ADDRESS_LEN];
                memmove (aucTempROM,                                   atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM, sizeof(aucTempROM));
                memmove (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, atdNewTempSensors[ONEWIRE_KRAN_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM));
                memmove (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, aucTempROM,                                 sizeof(atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM));
                NVM_vSaveSettings();
            }
            break;

        case MENU_FN_UNDEF:
        default:
            break;
    }
}
