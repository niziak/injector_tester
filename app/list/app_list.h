/*
 * app_edit.h
 *
 *  Created on: 22 paü 2013
 *      Author: nizinski_w
 */

#ifndef APP_LIST_H_
	#define APP_LIST_H_

#define LIST_DEBUG       1

#if (LIST_DEBUG)
  #define LIST_PRINTF(f,s...)       printf(f, ##s)
  #define LIST_PRINTF_P(f,s...)     printf_P(f, ##s)
  #define LIST_PRINTF_T_P(f,s...)   printf_P(f, ##s)
#else
  #define LIST_PRINTF(x,s...)
  #define LIST_PRINTF_P(x,s...)
  #define LIST_PRINTF_T_P(x,s...)
#endif

/**
 * Position of currently edited digit
 */
typedef enum
{
    AL_POS_FIRST   = 0,

    AL_POS_START_HOUR    = 0,
    AL_POS_START_MIN_10,
    AL_POS_START_MIN_1,

    AL_POS_END_HOUR,
    AL_POS_END_MIN_10,
    AL_POS_END_MIN_1,

    AL_POS_LAST
} APP_LIST_POS_DEF;

typedef struct
{
    APP_MODE_DEF        eEditedAppMode;      ///< mode which is edited
    MODE_SETTINGS_DEF   stMode;     ///< local copy of edited mode

    unsigned char ucLines;          ///< number of lines on list
    unsigned char ucCurrentLine;    ///< current position on list

    BOOL          bEditable;        ///< current line is in edit mode
    APP_LIST_POS_DEF eCurrentEditPos; ///< current position to edit

    unsigned char ucNewHour;        ///< tens part of hour
    unsigned char ucNewMin10;       ///< tens part of minutes
    unsigned char ucNewMin1;        ///< ones part of minutes
    unsigned char ucNewSec10;       ///< tens part of seconds
    unsigned char ucNewSec1;        ///< ones part of seconds
} APP_LIST_DEF;

extern APP_LIST_DEF    tdAppList;
#define ptdAppList     (&(tdAppList))

extern void APP_LIST_vHandleEvent(EVENT_DEF eEvent);
extern void APP_LIST_vShowDisplay(void);

#endif /* APP_LIST_H_ */
