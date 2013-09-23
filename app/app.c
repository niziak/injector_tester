/*
 * app.c
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#include <app.h>

static APP_ID_DEF   eActiveApp;

void APP_vUpdateDisplay(void)
{
    switch (eActiveApp)
    {
        default:
            RESET("No act appd");
            break;

        case APP_MENU:
            MENU_vShow();
            break;

        case APP_STATUS:
            DISP_vPrintStatusScreen(); // redraw status screen
            break;
    }
}

void APP_vRouteEvent(EVENT_DEF eEvent)
{
    switch (eActiveApp)
    {
        default:
            RESET("No act app");
            break;

        case APP_MENU:
            MENU_HandleEvent(eEvent);
            break;

        case APP_STATUS:
            switch (eEvent)
                {
                    case MENU_ACTION_SELECT:
                        APP_vActivateApp(APP_MENU);
                        MENU_Activate();                // menu not active - so activate it
                        break;

                    case MENU_ACTION_NEXT:
                        DISP_vStatusScreenNext();
                        break;

                    case SYS_UI_TIMEOUT:
                        DISP_vStatusScreenShow (STATUS_SCREEN_IDLE);
                        break;

                    default:
                        break;
                }
            break;
    }
}

void APP_vActivateApp(APP_ID_DEF   eNewActiveAppId)
{
    eActiveApp = eNewActiveAppId;
}
