/*
 * nvm.h
 *
 *  Created on: Sep 12, 2013
 *      Author: nizinski_w
 */

#ifndef NVM_H_
#define NVM_H_

#include <stdint.h>
#include <avr/eeprom.h>
#include <config.h>

typedef struct
{
    UCHAR   ucVersion;
} NVM_SET_DEF;


//static EEMEM NVM_SET_DEF        tdNVM;

extern void NVM_vLoadSettings(void);
extern void NVM_vSaveSettings(void);
extern void NVM_vRestoreFactory(void);

#endif /* NVM_H_ */
