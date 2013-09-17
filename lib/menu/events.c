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
#include <tools.h>

#if 0
    #define DEBUG(a)	LOG(a)
#else
    #define DEBUG(a)
#endif

#define EVENT_QUEUE_REAL_LEN (EVENT_QUEUE_LEN)+1

typedef struct
{
    MENU_EVENT_DEF      aeCurrentEvent[EVENT_QUEUE_REAL_LEN];
    UCHAR               ucReadPtr;
    UCHAR               ucWritePtr;
    BOOL                bFull;
    BOOL                bEmpty;
} FIFO_DEF;

static FIFO_DEF tdFifo;

#define ptdFifo (&tdFifo)

/**
 *  Initial state ucReadPtr = ucWritePtr = 0, bFull = FALSE;
 *  --- current implementation with fifo full flag
 *  Scenarios to handle
 *   1) ucReadPtr == ucWritePtr && bFull == FALSE   FIFO is empty
 *   2) ucReadPtr == EVENT_QUEUE_REAL_LEN           wraparound, ucReadPtr=0
 *   3) ucWritePtr == EVENT_QUEUE_REAL_LEN          wraparound, ucWritePtr=0
 *   4) bFull == TRUE                               fifo full, no write allowed
 *   5) after write: ucReadPtr == ucWritePTr        fifo full, set bFull flag
 *
 *
 *
 *  --- old implementation with walking hole to detect overflows:
 *  Scenarios to handle (previous implementation)
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


static void vDumpState(void)
{
    LCD_vClrScr();
    for (UCHAR a=0; a<sizeof(ptdFifo->aeCurrentEvent); a++ )
    {
        LCD_vPrintf("%X", (unsigned char)ptdFifo->aeCurrentEvent[a]);
    }
    LCD_vGotoXY(0,1);
    LCD_vPrintf("r%Xw%X", ptdFifo->ucReadPtr, ptdFifo->ucWritePtr);
}

//TODO make it real mutually exclusive
void EventPostFromIRQ (MENU_EVENT_DEF eEvent)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
//        if (    (ucWritePtr + 1 == ucReadPtr) // if write position is just before read, means fifo overflow possible
//             || ( (ucWritePtr + 1 == EVENT_QUEUE_LEN) && (ucReadPtr == 0) ) // the same but for wrap around
//           )
        if ((ptdFifo->ucWritePtr == ptdFifo->ucReadPtr) && (ptdFifo->bFull))
        {
            vDumpState();
            LCD_vPuts_P("Ffull ");
            int_delay_ms(300);
            return;
//            for (;;);
//            RESET("Fifo full");
        }

        ptdFifo->aeCurrentEvent[ptdFifo->ucWritePtr] = eEvent;
        ptdFifo->ucWritePtr++;
        if (EVENT_QUEUE_LEN == ptdFifo->ucWritePtr)
        {
            ptdFifo->ucWritePtr = 0;
        }
        // if next write position reach read pointer - fifo is full, block further writings
        if (ptdFifo->ucWritePtr == ptdFifo->ucReadPtr)
        {
            ptdFifo->bFull = TRUE;
        }
    }
}

void EventPost (MENU_EVENT_DEF eEvent)
{
    DEBUG("Post");
    EventPostFromIRQ(eEvent);
}

BOOL bIsEventWaiting(void)
{
    DEBUG("IsWait?");
    BOOL bReturn;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        //bReturn = (ptdFifo->bEmpty==TRUE ? FALSE : TRUE);
        bReturn = (ptdFifo->ucReadPtr == ptdFifo->ucWritePtr && ptdFifo->bFull==FALSE) ? FALSE : TRUE;
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
        eRet = ptdFifo->aeCurrentEvent[ptdFifo->ucReadPtr];
        ptdFifo->aeCurrentEvent[ptdFifo->ucReadPtr] = SYS_EVENT_NONE;
        ptdFifo->ucReadPtr++;
        if (EVENT_QUEUE_LEN == ptdFifo->ucReadPtr)
        {
            ptdFifo->ucReadPtr = 0;
        }
        ptdFifo->bFull = FALSE; // we got one element, so there is always space

//        // if after read, next element is equal to next write position, there is no more elements
//        if (ptdFifo->ucReadPtr == ptdFifo->ucWritePtr)
//        {
//            bFifoEmpty = TRUE;
//        }
//        else
//        {
//            bFifoEmpty = FALSE;
//        }
    }

	return eRet;
}

void EventInit(void)
{
    ptdFifo->ucReadPtr=0;
    ptdFifo->ucWritePtr=0;
    ptdFifo->bFull=0;
    ptdFifo->bEmpty=0;
    memset (ptdFifo->aeCurrentEvent, SYS_EVENT_NONE, sizeof(ptdFifo->aeCurrentEvent));
}
