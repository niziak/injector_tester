/*
 * 1w.h
 *
 *  Created on: 02-08-2011
 *      Author: nizinski_w
 */

#ifndef OW_LOW_H_
#define OW_LOW_H_

#include <config.h>
#include <stddef.h>
#include <limits.h>

#define DS1W_SEARCH_ROM         0xF0    /// search all devices
#define DS1W_ALARM_SEARCH       0xEC    /// search for devices in alerting state
#define DS1W_READ_ROM           0x33
#define DS1W_MATCH_ROM          0x55    ///< set 1wire device to read
#define DS1W_SKIP_ROM           0xCC
#define DS1W_CONVERT_T          0x44
#define DS1W_WRITE_SCRATCHPAD   0x4E
#define DS1W_READ_SCRATCHPAD    0xBE
#define DS1W_COPY_SCRATCHPAD    0x48
#define DS1W_RECALL_E2          0xB8
#define DS1W_READ_POWER_SUPPLY  0xB4

#define DS1W_RES_12             0x7F      // 0 1 1 1   1 1 1 1
#define DS1W_RES_11             0x5F      // 0 1 0 1   1 1 1 1
#define DS1W_RES_10             0x3F      // 0 0 1 1   1 1 1 1
#define DS1W_RES_9              0x1F      // 0 0 0 1   1 1 1 1

extern uint8_t OWInit(void);
extern uint8_t OWReadBit (void);
extern uint8_t OWReadByte (void);
extern void    OWWriteBit (uint8_t bit);
extern void    OWWriteByte (uint8_t b);
extern void    OWWriteBuf (uint8_t *buf, size_t len);
extern void    OWReadBuf (uint8_t *buf, size_t len);
extern int     OWSearchRom(uint8_t *ROM);

extern void    OWCalibrateDelays(void);

#endif /* OW_LOW_H_ */
