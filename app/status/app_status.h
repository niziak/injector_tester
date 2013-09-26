/*
 * display_status.h
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#ifndef DISPLAY_STATUS_H_
#define DISPLAY_STATUS_H_

typedef enum
{
    STATUS_SCREEN_IDLE=0,
    STATUS_SCREEN_NEW_SENSORS,
    STATUS_SCREEN_KNOWN_SENSORS,
    STATUS_SCREEN_TEMP,

    STATUS_SCREEN_LAST
} STATUS_SCREEEN_ID_DEF;

extern void DISP_vStatusScreenNext(void);
extern void DISP_vPrintStatusScreen(void);
extern void DISP_vStatusScreenSetNew(STATUS_SCREEEN_ID_DEF eNewScreenId);


#endif /* DISPLAY_STATUS_H_ */
