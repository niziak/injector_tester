/*
 * app.c
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#include <app.h>
#include <events.h>

#define APP_STACK_DEPTH     4

/**
 * Describes internal state of main application handler (application stack)
 */
typedef struct
{
    APP_ID_DEF      aeAppStack[APP_STACK_DEPTH];     ///< application stack
    unsigned char   ucStackTopNext;                  ///< index of next free place on stack [0..APP_STACK_DEPTH-1]
} APP_DEF;

static APP_DEF  tdApp;
#define ptdApp      (&(tdApp))

void APP_vInit(void)
{
    ptdApp->ucStackTopNext = 0;
}

/**
 * Redraw display of active application
 */
void APP_vUpdateDisplay(void)
{
    APP_ID_DEF eActiveApp = ptdApp->aeAppStack[ptdApp->ucStackTopNext-1];
    if (ptdApp->ucStackTopNext==0)
    {
        RESET("AppNoApp");
        return;
    }
    DEBUG_T_P(PSTR("\nAPP_vUpdateDisplay for %d\n"), eActiveApp);
    switch (eActiveApp)
    {
        default:
            RESET("AppNoActApp");
            break;

        case APP_MENU:
            MENU_vShow();
            break;

        case APP_STATUS:
            DISP_vPrintStatusScreen(); // redraw status screen
            break;

        case APP_LIST:
            APP_LIST_vShowDisplay();
            break;

        case APP_CLOCK:
            APP_CLOCK_vShow();
            break;

        case APP_POPUP:
            // do nothing, message was shown previously
            break;
    }
}

/**
 * Route event to active application
 * @param eEvent
 */
void APP_vRouteEvent(EVENT_DEF eEvent)
{
    APP_ID_DEF eActiveApp = ptdApp->aeAppStack[ptdApp->ucStackTopNext-1];
    if (ptdApp->ucStackTopNext==0)
    {
        RESET("AppNoApp");
        return;
    }
    DEBUG_P(PSTR("APP_vRouteEvent(%d) to %d app\n"), eEvent, eActiveApp);

    switch (eActiveApp)
    {
        default:
            RESET("AppNoActApp");
            break;

        case APP_CLOCK:
            APP_CLOCK_vHandleEvent(eEvent);
            break;

        case APP_MENU:
            MENU_HandleEvent(eEvent);
            break;

        case APP_STATUS:
            APP_STATUS_vHandleEvent(eEvent);
            break;

        case APP_LIST:
            APP_LIST_vHandleEvent(eEvent);
            break;

        case APP_POPUP:
            switch (eEvent)
            {
                // TODO: make keypress detection easier - maybe by bitmask ?
                case MENU_ACTION_UP:
                case MENU_ACTION_DOWN:
                case MENU_ACTION_LEFT:
                case MENU_ACTION_RIGHT:
                case MENU_ACTION_SELECT:
                case APP_POPUP_SHOWN:
                    EventTimerClear (EVENT_TIMER_POPUP);
                    APP_vReactivatePreviousApp();
                    APP_vRouteEvent(eEvent);
                    DISP_REFRESH
                    break;

                default:
                    break;
            }
            break;
    }

    //TODO make app stack to back from menu automatically to APP_STATUS
    switch (eEvent)
    {
        //TODO UI_TIMEOUT is handled inside each app - for future make RESET to preempt application
        case SYS_UI_TIMEOUT:
            // kill all application but not first one
            while (ptdApp->ucStackTopNext>1)
            {
                APP_vRouteEvent(APP_LOST_CONTROL);
                APP_vRouteEvent(APP_KILLED);
                ptdApp->ucStackTopNext--;
            }
            APP_vRouteEvent(APP_REACTIVATED);
            break;

        default:
            break;
    }


}

/**
 * Set activated application as current and call handler with event @ref APP_ACTIVATE
 * @param eNewActiveAppId
 */
void APP_vActivateApp(APP_ID_DEF   eNewActiveAppId)
{
    DEBUG_P(PSTR("APP_vActivateApp(%d)\n"), eNewActiveAppId);
    if (ptdApp->ucStackTopNext >= APP_STACK_DEPTH)
    {
        RESET("AppStackFull");
        return;
    }
    if (ptdApp->ucStackTopNext > 0)
    {
        APP_vRouteEvent(APP_LOST_CONTROL);
    }

    ptdApp->aeAppStack[ptdApp->ucStackTopNext] = eNewActiveAppId;
    ptdApp->ucStackTopNext++;

    APP_vRouteEvent(APP_ACTIVATED);
    DISP_STOP_BLINK_TIMER
}


void APP_vReactivatePreviousApp(void)
{
    DEBUG_P(PSTR("APP_vReactivatePreviousApp()\n"));
    if (ptdApp->ucStackTopNext==0)
    {
        RESET("AppReaStEmp");
        return;
    }
    APP_vRouteEvent(APP_LOST_CONTROL);

    ptdApp->ucStackTopNext--;

    APP_vRouteEvent(APP_REACTIVATED);
    DISP_STOP_BLINK_TIMER
}

void APP_vShowPopupMessage(const char *pcMessage, unsigned int delayms)
{
    LCD_vClrScr();
    LCD_vPrintf("%s", pcMessage);
    APP_vActivateApp(APP_POPUP);
    EventTimerPostAfter (EVENT_TIMER_POPUP, APP_POPUP_SHOWN, delayms);
}

void APP_vShowPopupMessage_P(const char *pcMessage, unsigned int delayms)
{
    LCD_vClrScr();
    LCD_vPrintf_P("%s", pcMessage);
    APP_vActivateApp(APP_POPUP);
    EventTimerPostAfter (EVENT_TIMER_POPUP, APP_POPUP_SHOWN, delayms);
}
