/*
 * error.h
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#ifndef LOG_H_
#define LOG_H_

#define PREFIX_LINE_FUNC              {printf_P(PSTR("%04d %-25s | "),__LINE__, __func__);}

#define RESET(string)	                LOG_Reset(string)
#define DEBUG(format, string...)        {                                           LOG_Log(format, ##string);}
#define DEBUG_T(format, string...)      {LOG_Log_P(PSTR("%5d "),ulSystemTickS);     LOG_Log(format, ##string);}
#define DEBUG_P(format, string...)      {                                           LOG_Log_P(format, ##string);}
#define DEBUG_T_P(format, string...)    {LOG_Log_P(PSTR("%5d "),ulSystemTickS);     LOG_Log_P(format, ##string);}
#define LOG(format, string...)          LOG_Log(format, ##string)

extern void LOG_Reset (char * message);
extern void LOG_Log   (const char *format, ...);
extern void LOG_Log_P   (const char *format, ...);

#endif /* LOG_H_ */
