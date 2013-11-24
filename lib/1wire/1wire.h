/*
 * 1w.h
 *
 *  Created on: 02-08-2011
 *      Author: nizinski_w
 */

#ifndef OW_H_
#define OW_H_

#include <limits.h>

extern void    OWDetectDevices(void);
extern void    OWCalibrateDelays(void);
extern void    OW_vWorker(void);
extern void    OW_vStartConversion(void) OW_GCC_OPT2;

extern UINT    uiOWGetTemp(UCHAR *pucROM);
extern void    TM_Convert_temperature(UCHAR idx,UINT *temp,UINT *frac);
extern void    OWSortList(void);

#endif /* OW_H_ */
