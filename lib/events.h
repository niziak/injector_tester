/*
 * events.h
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include <types.h>

typedef enum
{
    EVENT_TIMER_1WIRE = 0,  ///< timer id, first must be zero because it is used as index in @atdTimers

    EVENT_TIMER_LAST,
} EVENT_DELAYED_TIMER_ID;



#define EVENT_QUEUE_LEN 16
typedef enum
{
    SYS_EVENT_NONE =0,

    SYS_HEARTBEAT,

    SYS_1WIRE_CONVERT,
    SYS_1WIRE_READ,


    SYS_UI_TIMEOUT,            ///< no user action - timeout from last keypress

    MENU_ACTION_NEXT,           ///< navigagte to next element
    MENU_ACTION_SELECT,         ///< make action on selected element
    MENU_ACTION_CONFIRMED,
//    MENU_ACTION_PAUSE,          ///< disable menu, but don't clear internal structure
//    MENU_ACTION_RESUME,         ///< enable menu, but don't reset internal structures

    DISP_UPDATE,                ///< redraw display

} EVENT_DEF;

extern void EventPost (EVENT_DEF eEvent);
extern void EventPostFromIRQ (EVENT_DEF eEvent);
extern void EventTimerPostAFter (EVENT_DELAYED_TIMER_ID eTimerId, EVENT_DEF eEvent, unsigned int delayms);
extern void EventTimerTickEveryMS(void);
extern BOOL bIsEventWaiting(void);
extern EVENT_DEF EventGet(void);
extern void EventInit(void);

#endif /* EVENTS_H_ */
