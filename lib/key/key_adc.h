/*
 * adc.h
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#ifndef KEY_ADC_H_
#define KEY_ADC_H_

#if (USE_ARDUINO_ADC_KEYPAD)

extern void KEY_ADC_vInit(void);
extern volatile unsigned char ucADC;

#endif

#endif /* KEY_ADC_H_ */
