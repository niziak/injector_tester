/*
 * events.c
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include "menu.h"
#include <log.h>
#include <util/atomic.h>
#include <string.h>

//#define DEBUG(a)	LOG(a)
#define DEBUG(a)

#define EVENT_QUEUE_REAL_LEN (EVENT_QUEUE_LEN)+1
static MENU_EVENT_DEF	aeCurrentEvent[EVENT_QUEUE_REAL_LEN];
static UCHAR  ucReadPtr, ucWritePtr;
/**
 *  Initial state ucReadPtr = ucWritePtr = 0
 *  Scenarios to handle
 *   1) ucReadPtr == ucWritePtr                 FIFO is empty
 *   2) ucReadPtr == EVENT_QUEUE_REAL_LEN       wraparound, ucReadPtr=0
 *   3) ucWritePtr == EVENT_QUEUE_REAL_LEN      wraparound, ucWritePtr=0
 *   4) ucWritePtr+1 = ucReadPtr                full, cannot write. Affter write read==write, which means empty, but it will be completely full
 *                                              so one element space is left between read and write pointers
 *   5) above with wrap around
 *
 *   During work there is always one element hole between read and write ptr to detect FIFO full
 *   Other implementation (without hole) requires additional fifo full flag
 */


//TODO make it real mutually exclusive
void EventPostFromIRQ (MENU_EVENT_DEF eEvent)
{
    EventPost(eEvent);
}

void EventPost (MENU_EVENT_DEF eEvent)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if (    (ucWritePtr + 1 == ucReadPtr) // if write position is just before read, means fifo overflow possible
             || ( (ucWritePtr + 1 == EVENT_QUEUE_LEN) && (ucReadPtr == 0) ) // the same but for wrap around
           )
        {
            RESET("Fifo full");
        }

        aeCurrentEvent[ucWritePtr++] = eEvent;
        if (EVENT_QUEUE_LEN == ucWritePtr)
        {
            ucWritePtr = 0;
        }
    }
}

BOOL bIsEventWaiting(void)
{
    BOOL bReturn;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        bReturn = (ucReadPtr != ucWritePtr ? TRUE : FALSE);
    }
	return bReturn;
}

MENU_EVENT_DEF EventGet(void)
{
    MENU_EVENT_DEF eRet;
    DEBUG("Get");
    if (bIsEventWaiting()==FALSE)
    {
        RESET("GET but no ev");
        return SYS_EVENT_NONE;
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        eRet = aeCurrentEvent[ucReadPtr++];
        if (EVENT_QUEUE_LEN == ucReadPtr)
        {
            ucReadPtr = 0;
        }
    }

	return eRet;
}

void EventInit(void)
{
    ucReadPtr=ucWritePtr=0;
    memset (aeCurrentEvent, SYS_EVENT_NONE, sizeof(aeCurrentEvent));
}
