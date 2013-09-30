/*
 * key.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#include <avr/io.h>
#include <util/delay.h>

#include <config.h>
#include <key.h>
#include <app.h>
#include <log.h>
#include <tools.h>

#if (USE_SINGLE_PIN_BUTTONS)
volatile unsigned char ucKeyOkState;        ///< 0 if key released. Incremented if key pressed
volatile unsigned char ucKeyNextState;      ///< 0 if key released. Incremented if key pressed
volatile unsigned char ucKeyBlocked;        ///< 0 - unblocked, automatically decremented

/**
 * Keyscan routine, must be called from IRQ, i.e. every 10 ms
 */
void KEY_vKeyIsr(void)
{
    if (ucKeyBlocked>0)
    {
        ucKeyBlocked--;
        return;
    }
    if (BTN_NEXT_PRESSED)
    {
        ucKeyNextState++;
    }
    else
    {
        if (ucKeyNextState > KEY_DEBOUNCE)
        {
            EventPostFromIRQ(MENU_ACTION_DOWN);
            ucKeyBlocked = KEY_INTERVAL;
            ADC_KEY_PRESSED_TRIGGER_FN;
        }
        ucKeyNextState = 0;
    }

    if (BTN_OK_PRESSED)
    {
        ucKeyOkState++;
    }
    else
    {
        if (ucKeyOkState > KEY_DEBOUNCE)
        {
            EventPostFromIRQ(MENU_ACTION_SELECT);
            ucKeyBlocked = KEY_INTERVAL;
            ADC_KEY_PRESSED_TRIGGER_FN;
        }
        ucKeyOkState = 0;
    }

    // if there is no events, read keyboard
//    if (FALSE==bEventCheck())
//    {
//        KEY_ReadKeyboard();
//    }
}

//void KEY_ReadKeyboard(void)
//{
//    if (ucCounter>2)
//    {
//        // when key change from 0 to 1 - releasing key
//        if ((bKeyStartOldState==0) && (BTN_OK_GETSTATE==1))
//        {
//            EventPostFromIRQ(MENU_ACTION_SELECT);
//        }
//        else // no more than 1 key at time
//        if ((bKeySetOldState==0) && (BTN_NEXT_GETSTATE==1))
//        {
//            EventPostFromIRQ(MENU_ACTION_NEXT);
//        }
//    }
//
//    if ((BTN_OK_GETSTATE==0) || (BTN_NEXT_GETSTATE==0))
//    {
//        ucCounter++;
//    }
//    else
//    {
//        ucCounter=0;
//    };
//
//    bKeySetOldState = BTN_NEXT_GETSTATE;
//    bKeyStartOldState = BTN_OK_GETSTATE;
//}

void KEY_vInit(void)
{
	BTN_OK_DDR    &= ~_BV(BTN_OK_PIN  );
	BTN_NEXT_DDR  &= ~_BV(BTN_NEXT_PIN);

	BTN_OK_PORT   |=  _BV(BTN_OK_PIN  );
	BTN_NEXT_PORT |=  _BV(BTN_NEXT_PIN);

	ucKeyOkState    = 0;
	ucKeyNextState  = 0;
	ucKeyBlocked    = 0;

}

#endif //(USE_SINGLE_PIN_BUTTONS)
