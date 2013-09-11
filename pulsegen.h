/*
 * puslegen.h
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef PULSEGEN_H_
#define PULSEGEN_H_

#define OC2_PORT	PORTB
#define OC2_DDR		DDRB
#define OC2_PIN		PINB3

#define OC2_LOW		{	OC2_PORT &=~ _BV(OC2_PIN); }
#define OC2_HI		{	OC2_PORT |=  _BV(OC2_PIN); }
#define OC2_SETUP	{	OC2_DDR  |=  _BV(OC2_PIN); }

#define	OC2_PRESCALE	1024


#define ONOFF_PORT		PORTB
#define ONOFF_DDR		DDRB
#define ONOFF_PIN		PINB4

#define ONOFF_LOW		{	ONOFF_PORT &=~ _BV(ONOFF_PIN); }
#define ONOFF_HI	 	{	ONOFF_PORT |=  _BV(ONOFF_PIN); }
#define ONOFF_SETUP		{   ONOFF_DDR  |=  _BV(ONOFF_PIN); }


extern void vPulseGenApplication (void);

#endif /* PULSEGEN_H_ */
