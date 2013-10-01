/*
 * app.h
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#ifndef _APP_H_
    #define _APP_H_

#include <events.h>
#include <app_status.h>
#include <app_menu.h>
#include <app_clock.h>

/**
 * Application ID definitions
 */
typedef enum
{
    APP_INVALID,
    APP_MENU,
    APP_STATUS,
    APP_CLOCK,
    APP_POPUP,
} APP_ID_DEF;

extern void APP_vInit(void);
extern void APP_vRouteEvent(EVENT_DEF eEvent);
extern void APP_vUpdateDisplay(void);

extern void APP_vActivateApp(APP_ID_DEF   eNewActiveAppId);
extern void APP_vReactivatePreviousApp(void);

extern void APP_vShowPopupMessage(const char *pcMessage, unsigned int delayms);

#endif // _APP_H_
