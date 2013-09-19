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
    // DISPLAY HANDLING
    if (TRUE == MENU_bIsMenuActive())
    {
        MENU_vShow(); // redraw menu
    }
    else
    {
        DISP_vPrintStatusScreen(); // redraw status screen
    }
}

void APP_vRouteEvent(EVENT_DEF eEvent)
{
    switch (eActiveApp)
    {
        default:
            RESET("No act app");
            break;

        case APP_STATUS:
            if (TRUE == MENU_bIsMenuActive())
            {
                MENU_HandleEvent(eEvent);   // handle menu events if menu active
            }
            else
            {
                switch (eEvent)
                {
                    case MENU_ACTION_SELECT:
                        MENU_Activate();                // menu not active - so activate it
                        break;

                    case MENU_ACTION_NEXT:
                        DISP_vStatusScreenNext();
                        break;

                    case SYS_UI_TIMEOUT:
                        LOG("UI TO");
                        DISP_vStatusScreenShow (STATUS_SCREEN_IDLE);
                        break;

                    default:
                        break;
                }
            }
            break;
    }
}

void APP_vActivateApp(APP_ID_DEF   eNewActiveAppId)
{
    eActiveApp = eNewActiveAppId;
}
