/*
 * key.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#include <avr/io.h>
#include <util/delay.h>
#include <key.h>
#include <lib/menu/menu.h>
#include <log.h>

static unsigned char bKeyStartOldState = 1;	// pulledup
static unsigned char bKeySetOldState 	= 1;	// pulledup

void KEY_ReadKeyboard(void)
{
	// when key change from 0 to 1 - releasing key
	if ((bKeyStartOldState==0) && (BTN_START_GETSTATE==1))
	{
		_delay_ms(30);
		if (BTN_START_GETSTATE==1)
		{
			EventPost(MENU_ACTION_SELECT);
		}
	}

	if ((bKeySetOldState==0) && (BTN_SET_GETSTATE==1))
	{
		_delay_ms(30);
		if (BTN_SET_GETSTATE==1)
		{
			EventPost(MENU_ACTION_NEXT);
		}
	}

	bKeySetOldState = BTN_SET_GETSTATE;
	bKeyStartOldState = BTN_START_GETSTATE;
}

void KEY_vInit(void)
{
	BTN_OK_DDR &= ~_BV(BTN_OK_PIN);
	BTN_SET_DDR &= ~_BV(BTN_SET_PIN);
	BTN_OK_PORT |= _BV(BTN_OK_PIN);
	BTN_SET_PORT |= _BV(BTN_SET_PIN);

}
