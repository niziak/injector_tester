/*
 * error.h
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#ifndef LOG_H_
#define LOG_H_

#define PREFIX_LINE_FUNC              {printf_P(PSTR("%04d %-25s | "),__LINE__, __func__);}

#define RESET(string)	LOG_Reset(string)

extern void LOG_Reset (char * message);
extern void LOG       (char * message);

#endif /* LOG_H_ */
