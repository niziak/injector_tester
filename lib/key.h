/*
 * key.h
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef KEY_H_
#define KEY_H_

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

#define KEY_DEBOUNCE        3       ///< n*10ms     how many cycles wait for key unchanged state
#define KEY_INTERVAL        3       ///< n*10ms     how long block keyboard reading after keypress

extern void KEY_ReadKeyboard(void);
extern void KEY_vInit(void);
extern void KEY_vKeyIsr(void);

extern volatile unsigned char ucKeyOkState;
extern volatile unsigned char ucKeyNextState;
extern volatile unsigned char ucKeyBlocked;

#endif /* KEY_H_ */
