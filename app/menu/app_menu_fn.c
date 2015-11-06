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
#include <texts.h>
#include <app_list.h>

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
        case MENU_FN_SETTINGS_AUTO_5:
            ptdAppList->ucLines = 5;
            ptdAppList->eEditedAppMode = APP_MODE_AUTO_2 /* ZERO */ + ( eFunctionId - MENU_FN_SETTINGS_AUTO_2 );
            ptdAppList->ucCurrentLine = 0;
            APP_vActivateApp(APP_LIST);
            break;

        case MENU_FN_APP_MODE_24H:
        case MENU_FN_APP_MODE_AUTO_1:
        case MENU_FN_APP_MODE_AUTO_2:
        case MENU_FN_APP_MODE_AUTO_3:
        case MENU_FN_APP_MODE_AUTO_4:
        case MENU_FN_APP_MODE_AUTO_5:
            // Dirty calculation of mode
            pstSettings->eAppMode = APP_MODE_24H /* ZERO */ + ( eFunctionId - MENU_FN_APP_MODE_24H );
            NVM_vSaveSettings();
            APP_vShowPopupMessage_P(PSTR_TXT_MODE_CHANGED, UI_POS_POPUP_TIME);
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
                uiPumpSwitchOffAfter = pstSettings->uiPumpManualTime;
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
                APP_vShowPopupMessage_P(PSTR_TXT_NO_NEW_SENSORS, UI_NEG_POPUP_TIME);
            }
            else
            {
                memmove (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM));
                memmove (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, atdNewTempSensors[ONEWIRE_KRAN_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM));
                memset  (atdNewTempSensors, 0, sizeof(atdNewTempSensors));
                NVM_vSaveSettings();
                APP_vShowPopupMessage_P(PSTR_TXT_NEW_SENSORS, UI_POS_POPUP_TIME);
            }
            break;

        case MENU_FN_1W_DELETE:
            memset (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, 0, sizeof(atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM));
            memset (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, 0, sizeof(atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM));
            NVM_vSaveSettings();
            APP_vShowPopupMessage_P(PSTR_TXT_DELETED, UI_POS_POPUP_TIME);
            break;

        case MENU_FN_1W_CHANGE:
            {
                UCHAR aucTempROM[OW_ADDRESS_LEN];
                memmove (aucTempROM,                                   atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, sizeof(aucTempROM));
                memmove (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM, atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, sizeof(atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM));
                memmove (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM, aucTempROM,                                   sizeof(atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM));
                NVM_vSaveSettings();
                APP_vShowPopupMessage_P(PSTR_TXT_CHANGED, UI_POS_POPUP_TIME);
            }
            break;

        case MENU_FN_UNDEF:
        default:
            break;
    }
}
