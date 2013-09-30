/*
 * app.c
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#include <app.h>

static APP_ID_DEF   eActiveApp;     ///< active application ID

/**
 * Redraw display of active application
 */
void APP_vUpdateDisplay(void)
{
    DEBUG_T_P(PSTR("\nAPP_vUpdateDisplay\n"));
    switch (eActiveApp)
    {
        default:
            RESET("app No act appd");
            break;

        case APP_MENU:
            MENU_vShow();
            break;

        case APP_STATUS:
            DISP_vPrintStatusScreen(); // redraw status screen
            break;
    }
}

/**
 * Route event to active application
 * @param eEvent
 */
void APP_vRouteEvent(EVENT_DEF eEvent)
{
    DEBUG_P(PSTR("APP_vRouteEvent(%d) to %d app\n"), eEvent, eActiveApp);

    // TODO make app stack?
    switch (eEvent)
    {
        case APP_LOST_CONTROL:
            APP_vActivateApp(APP_STATUS);
            return;
            break;

        default:
            break;
    }

    switch (eActiveApp)
    {
        default:
            RESET("app No act app");
            break;

        case APP_MENU:
            MENU_HandleEvent(eEvent);
            break;

        case APP_STATUS:
            switch (eEvent)
                {
                    case APP_ACTIVATE:
                        DISP_vStatusScreenSetNew(STATUS_SCREEN_IDLE);
                        break;

                    case MENU_ACTION_SELECT:
                        APP_vActivateApp(APP_MENU);
                        break;

                    case MENU_ACTION_RIGHT:
                    case MENU_ACTION_DOWN:
                        DISP_vStatusScreenNext();
                        break;

                    case MENU_ACTION_LEFT:
                    case MENU_ACTION_UP:
                        DISP_vStatusScreenPrev();
                        break;

                    case SYS_UI_TIMEOUT:
                        DISP_vStatusScreenSetNew (STATUS_SCREEN_IDLE);
                        break;

                    default:
                        break;
                }
            break;
    }

    //TODO make app stack to back from menu automatically to APP_STATUS
    switch (eEvent)
    {
        //TODO UI_TIMEOUT is hanlded inside each app - for future make RESET to preempt application
        case SYS_UI_TIMEOUT:
            //DISP_vStatusScreenSetNew(STATUS_SCREEN_IDLE);
           // APP_vActivateApp(APP_STATUS);
            break;

        default:
            break;
    }


}

void APP_vActivateApp(APP_ID_DEF   eNewActiveAppId)
{
    eActiveApp = eNewActiveAppId;
    APP_vRouteEvent(APP_ACTIVATE);
}
