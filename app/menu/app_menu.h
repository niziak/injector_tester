/*
 * menu.h
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef MENU_H_
#define MENU_H_

#include <avr/pgmspace.h>

#include <config.h>
#include <types.h>
#include <lib/events.h>
#include <app_menu_config.h>

typedef enum
{
	MID_INVALID,
	MID_SETTINGS,
	MID_1W_ZASOBNIK,
	MID_1W_KRAN,
	MID_1W_KRAN_POKAZ,
	MID_1W_KRAN_USTAW,
    MID_1W_ZASOBNIK_POKAZ,
    MID_1W_ZASOBNIK_USTAW,
} MENU_ITEM_ID_DEF;

typedef enum
{
	LVL_UNDEF,
	LVL0,
	LVL1,
	LVL2,
	LVL3
} MENU_LEVEL_ID_DEF;

typedef enum
{
	MENU_FN_UNDEF,

	MENU_FN_CHILD_MENU,


	MENU_FN_REBOOT,
	MENU_FN_10MIN,
	MENU_FN_CLOCK,

	MENU_FN_SHOW_1W_ZASOBNIK,
	MENU_FN_SET_1W_ZASOBNIK,
	MENU_FN_SHOW_1W_KRAN,
	MENU_FN_SET_1W_KRAN,

	MENU_FN_FACTORY_DEFAULT,

} MENU_FN_ID_DEF;

typedef enum
{
	MENU_CONFIRM_UNDEF,
	ASK,
	NO_ASK,
} MENU_CONFIRM_DEF;


typedef struct
{
	MENU_LEVEL_ID_DEF		eLevel;
	MENU_ITEM_ID_DEF		eMID;
	PGM_P					pcLabel;
	MENU_FN_ID_DEF			eMenuFnId;
	MENU_CONFIRM_DEF    	eConfirmation;
} MENU_ITEM_DEF;

#define MENU_ITEM_ID_FIRST      0x00
#define MENU_ITEM_ID_NOT_FOUND  0xFF


#define NUMBER_OF_MENU_ITEMS		15
extern const MENU_ITEM_DEF				atdMenuItems[NUMBER_OF_MENU_ITEMS];
//#define NUMBER_OF_MENU_ITEMS		(sizeof(atdMenuItems)/sizeof(atdMenuItems[0]))

/**
 * Describes state of menu application instance
 */
typedef struct
{
    BOOL    bMenuActive;
    UCHAR   ucCurrentItem;          ///< Currently selected menu item (indexed from 0). Can be used as index for atdMenuItems[]
    BOOL    bEndMarkerSelected;     ///< end marker should be shown instead of last entry (it covers last entry)

    BOOL    bConfirmationScreenActive;
    BOOL    bConfirmationStateIsNo;    ///<    From YES/NO choice, NO is currently selected
} MENU_DEF;


#define PTDMENU         (&(tdMenu))
#define PCURRENT_ITEM   (&(atdMenuItems[PTDMENU->ucCurrentItem]))
extern MENU_DEF tdMenu;

extern void MENU_vInit(void);
extern void MENU_HandleEvent(EVENT_DEF eMenuEvent);
extern BOOL MENU_bIsMenuActive(void);
extern void MENU_vShow(void);
extern void MENU_Activate(void);
extern void MENU_Deactivate(void);

// internals:
extern void MENU_vDoFunction(MENU_FN_ID_DEF eFunctionId);
extern void MENU_ConfirmationScreenHandler(EVENT_DEF eMenuAction);
extern void MENU_MenuNavigationHandler(EVENT_DEF eMenuEvent);
extern MENU_LEVEL_ID_DEF MENU_eGetCurrentItemLevel(void);
extern UCHAR MENU_ucGetParentItem(UCHAR ucCurrentItem);

#endif /* MENU_H_ */
