/*
 * key.h
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef KEY_BUTTONS_H_
#define KEY_BUTTONS_H_

#if (USE_SINGLE_PIN_BUTTONS)

#define BTN_OK_PORT		PORTB
#define BTN_OK_DDR		DDRB
#define BTN_OK_PIN		PINB1

#define BTN_NEXT_PORT	PORTB
#define BTN_NEXT_DDR	DDRB
#define BTN_NEXT_PIN	PINB2

#define BTN_NEXT_GETSTATE	( (PINB & _BV(BTN_NEXT_PIN)) == _BV(BTN_NEXT_PIN) ? 1 : 0 )
#define BTN_OK_GETSTATE     ( (PINB & _BV(BTN_OK_PIN))   == _BV(BTN_OK_PIN)   ? 1 : 0 )

#define BTN_NEXT_PRESSED     ( BTN_NEXT_GETSTATE == 0 )
#define BTN_OK_PRESSED       ( BTN_OK_GETSTATE   == 0 )



extern void KEY_ReadKeyboard(void);
extern void KEY_vInit(void);
extern void KEY_vKeyIsr(void);

extern volatile unsigned char ucKeyOkState;
extern volatile unsigned char ucKeyNextState;
extern volatile unsigned char ucKeyBlocked;

#endif

#endif /* KEY_BUTTONS_H_ */
