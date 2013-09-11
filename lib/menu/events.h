/*
 * events.h
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include <types.h>
#include "menu.h"

typedef enum
{
    SYS_EVENT_NONE =0,

    MENU_ACTION_NONE,           ///< invalid value
    MENU_ACTION_NEXT,           ///< navigagte to next element
    MENU_ACTION_SELECT,         ///< make action on selected element
    MENU_ACTION_PAUSE,          ///< disable menu, but don't clear internal structure
    MENU_ACTION_RESUME,         ///< enable menu, but don't reset internal structures

    MENU_ACTION_CONFIRMED,
} MENU_EVENT_DEF;

extern void EventPost (MENU_EVENT_DEF eEvent);
extern BOOL bEventCheck(void);
extern MENU_EVENT_DEF EventGet(void);
extern void EventClear(void);

#endif /* EVENTS_H_ */
