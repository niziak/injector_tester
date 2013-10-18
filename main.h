/*
 * main.h
 *
 *  Created on: Oct 8, 2013
 *      Author: nizinski_w
 */

#ifndef MAIN_H_
#define MAIN_H_

extern volatile unsigned long       ulSystemTickMS;
extern volatile unsigned long       ulSystemTickS;
extern volatile BOOL                bBlinkState;
extern volatile UCHAR               ucUIInactiveCounter;
extern volatile unsigned int        uiPumpSwitchOffAfter;
extern volatile BOOL                bRefreshDisplay;
extern volatile BOOL                bNeedsBlinking;
extern volatile unsigned int        uiPIRTTL;
extern volatile BOOL                bPumpIsRunning;

#define DISP_REFRESH                { bRefreshDisplay = TRUE;  }
#define DISP_START_BLINK_TIMER      { bNeedsBlinking  = TRUE;  }
#define DISP_STOP_BLINK_TIMER       { bNeedsBlinking  = FALSE; }



#endif /* MAIN_H_ */
