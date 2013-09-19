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
#include <util/delay.h>

#define INJECTOR_TESTER_MODE	0

#define ONEWIRE_ZASO_IDX	0
#define ONEWIRE_KRAN_IDX		1

#define UI_INACTIVE_TIMEOUT     60          ///< (in seconds) go to idle mode when no keypresse in 60 seconds

extern volatile unsigned long ulSystemTickMS;
extern volatile unsigned long ulSystemTickS;
extern volatile BOOL bBlinkState;
extern volatile UCHAR ucUIInactiveCounter;
extern volatile unsigned int uiPumpSwitchOffAfter;
extern volatile BOOL bRefreshDisplay;

#endif /* CONFIG_H_ */
