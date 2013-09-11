/*
 * events.c
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include "menu.h"
#include <log.h>

//#define DEBUG(a)	LOG(a)
#define DEBUG(a)


static MENU_EVENT_DEF	eCurrentEvent;

void EventPost (MENU_EVENT_DEF eEvent)
{
    DEBUG("Post");
	if (eCurrentEvent != SYS_EVENT_NONE)
	{
		RESET("Unr evnt");
	}
	eCurrentEvent = eEvent;
}

BOOL bEventCheck(void)
{
	return (eCurrentEvent==SYS_EVENT_NONE ? FALSE : TRUE);
}

MENU_EVENT_DEF EventGet(void)
{
    DEBUG("Get");
    MENU_EVENT_DEF eRet = eCurrentEvent;
    EventClear();
	return eRet;
}

void EventClear(void)
{
	DEBUG("Clear");
	eCurrentEvent = SYS_EVENT_NONE;
}
