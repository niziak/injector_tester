/*
 * config.h
 *
 *  Created on: Aug 9, 2013
 *      Author: nizinski_w
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <types.h>
#include <log.h>

#define INJECTOR_TESTER_MODE	0

#define ONEWIRE_ZASOBNIK_IDX	0
#define ONEWIRE_KRAN_IDX		1

extern volatile unsigned long ulSystemTickMS;
extern volatile unsigned long ulSystemTickS;
extern volatile BOOL bBlinkState;


#endif /* CONFIG_H_ */
