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

#define BTN_SET_PORT		PORTB
#define BTN_SET_DDR			DDRB
#define BTN_SET_PIN			PINB2

#define BTN_SET_GETSTATE	( (PINB & _BV(BTN_SET_PIN)) == _BV(BTN_SET_PIN) ? 1 : 0 )
#define BTN_START_GETSTATE	( (PINB & _BV(BTN_OK_PIN)) == _BV(BTN_OK_PIN) ? 1 : 0 )



extern void KEY_ReadKeyboard(void);
extern void KEY_vInit(void);

#endif /* KEY_H_ */
