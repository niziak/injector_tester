/*
 * rtc.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#include <avr/pgmspace.h>
#include <util/atomic.h>

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "rtc.h"
#include <i2cmaster.h>


//#define DS1307_ADDR            0x68    // 104=0x68
/**
 * !!! i2cmaster is using 8 bit addresses, so shift your addres left
 */
#define DS1307_ADDR                 0xD0    // 1101000R = 0xD0


#define DS1307_REG_SECONDS          0        ///< BCD seconds (00-59)
    #define REG_SECONDS_MASK        0x7F     ///< seconds mask
    #define REG_SECONDS_CLOCK_HALT  7


#define DS1307_REG_MINUTES          1        ///< BCD minutes (00-59)

/** hours value must be re-set again when 12/24 MODE is changed */
#define DS1307_REG_HOURS            2        ///< BCD hours (1-12 / 00-23)
    #define REG_HOURS_12H_MODE_MASK 0x1F     ///<
    #define REG_HOURS_24H_MODE_MASK 0x3F     ///<
    #define REG_HOURS_12H_MODE      6        ///< when set, hours register works in 12h clock mode (from 1 to 12 with AM/PM flag), otherwise 24h mode from 00 to 23
    #define REG_HOURS_12H_PM        5        ///< PM when set , only for 12H mode


#define DS1307_REG_WDAY             3        ///< day of week (from 01 to 07)
    #define REG_WDAY_MASK           0x07     ///< mask not needed, other bits are zero

#define DS1307_REG_DAY              4        ///< BCD day of month (from 01-31)
    #define REG_DAY_MASK            0x3F     ///< mask not needed, other bits are zero

#define DS1307_REG_MONTH            5        ///< BCD month (from 01 to 12)
    #define REG_MONTH_MASK          0x1F     ///< mask not needed, other bits are zero

#define DS1307_REG_YEAR             6        ///< BCD year (00-99)
    #define REG_YEAR_MASK           0xFF

#define DS1307_REG_CONTROL          7
  #define CTRL_OUT_STATE       7    ///< SQ output state when @ref CTRL_SQW_ENABLE=0
  #define CTRL_SQW_ENABLE      4    ///< enable square wave generation
  #define CTRL_RS1             1    ///< frequency control of sqare wave
  #define CTRL_RS0             0    ///< frequency control of sqare wave

  #define CTRL_RS_MASK         3
  #define CTRL_RS_1HZ          0
  #define CTRL_RS_4096HZ       1
  #define CTRL_RS_8192HZ       2
  #define CTRL_RS_32768HZ      3

#define DS1307_RAM_START        0x08
#define DS1307_RAM_END          0x3F

#define RTC_DEBUG       1

#if (RTC_DEBUG)
  #define RTC_PRINTF(f,s...)     printf(f, ##s)
  #define RTC_PRINTF_P(f,s...)   printf_P(f, ##s)
  #define RTC_PRINTF_T_P(f,s...) printf_P(f, ##s)
#else
  #define RTC_PRINTF(x,s...)
  #define RTC_PRINTF_P(x,s...)
  #define RTC_PRINTF_T_P(x,s...)
#endif

tm          tdLocalTime;                ///< actual time in tm structure
time_t      tSecondsUntilEpoch;         ///< actual time in seconds until 1970
time_t      tOffsetFrom1970;


/**
 * Check halt flag of RTC clock
 * @return TRUE if clock is running
 */
BOOL RTC_bGetState(void)
{
    unsigned char ucByte;

    RTC_PRINTF_P(PSTR("RTC_bGetState(): "));

    i2c_start_wait(DS1307_ADDR+I2C_WRITE);
    if (1 == i2c_write(DS1307_REG_SECONDS))
    {
        RESET("I2C write!");
        return FALSE;
    }
    if (0 == i2c_rep_start(DS1307_ADDR+I2C_READ))
    {
        RESET("I2C i2c_rep_start!");
        return FALSE;
    }
    ucByte = i2c_readNak();
    i2c_stop();
    RTC_PRINTF_P(PSTR("%d\n"), ucByte);
    return ( (ucByte & (1<<REG_SECONDS_CLOCK_HALT)) == (1<<REG_SECONDS_CLOCK_HALT) ? 0 : 1);
}

/**
 * Start or stop clock by setting halt flag
 * @param bNewState - TRUE - clock running
 */
void RTC_vSetState(BOOL bNewState)
{
    unsigned char ucSecondsRegister;

    RTC_PRINTF_P(PSTR("RTC_vSetState(%d)\n"), bNewState);

    i2c_start_wait(DS1307_ADDR+I2C_WRITE);
    if (1 == i2c_write(DS1307_REG_SECONDS))
    {
       RESET("I2C write!");
       return;
    }
    if (0 == i2c_rep_start(DS1307_ADDR+I2C_READ))
    {
       RESET("I2C i2c_rep_start!");
       return;
    }
    ucSecondsRegister = i2c_readNak();

    if (bNewState==TRUE)
    {
        // enable clock - reset halt flag
        ucSecondsRegister &= ~(1<<REG_SECONDS_CLOCK_HALT);
    }
    else
    {
        // disable clock - set halt flag
        ucSecondsRegister |= (1<<REG_SECONDS_CLOCK_HALT);
    }

    if (0 == i2c_rep_start (DS1307_ADDR+I2C_WRITE))
    {
        RESET("I2C i2c_rep_start!");
        return;
    }
    if (1 == i2c_write(DS1307_REG_SECONDS))
    {
        RESET("I2C write!");
        return;
    }
    if (1 == i2c_write(ucSecondsRegister))
    {
        RESET("I2C write!");
        return;
    }
    i2c_stop();
}

void RTC_vInit(void)
{
    memset (&tdLocalTime,0, sizeof(tdLocalTime));
    tSecondsUntilEpoch = 0;

    DDRC   &= ~_BV(PINC5);      // input
    PORTC |=   _BV(PINC5);      // pull up

    DDRC   &= ~_BV(PINC4);      // input
    PORTC |=   _BV(PINC4);      // pull up


    i2c_init();                             // initialize I2C library
}

/**
 * Get system time
 *
 *
 * @return Number of seconds elapsed since 1970
 */
time_t time(void)
{
    return tOffsetFrom1970 + ulSystemTickS;
}

void stime(time_t *newTime)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        tOffsetFrom1970 = (*newTime) - ulSystemTickS;
    }
}


/**
 * Converts seconds until 1970 stored in @ref tNow to time struct @ref stored in @ref tdLocalTime
 */
void RTC_vConvertLocalTime(void)
{
//    ptdLocalTime->tm_hour = tSecondsUntilEpoch / SEC_PER_HOUR;
//    ptdLocalTime->tm_sec  = tSecondsUntilEpoch;
}


void RTC_vTickLocalTime(void)
{
    if (ptdLocalTime->tm_sec++ > SEC_PER_MIN)
    {
        ptdLocalTime->tm_sec = 0;
        if (ptdLocalTime->tm_min++ > MIN_PER_HOUR)
        {
            ptdLocalTime->tm_min = 0;
            if (ptdLocalTime->tm_hour++ > HOUR_PER_DAY)
            {
                ptdLocalTime->tm_hour = 0;
                ptdLocalTime->tm_mday++;
            }
        }
    }
}


void RTC_vGetTime(void)
{
    unsigned char aucData[(DS1307_REG_YEAR)-(DS1307_REG_SECONDS)+1];    // buffer to load registers from 0 (seconds) to 6 (year)
    unsigned char ucI;

    RTC_PRINTF_P (PSTR("RTC_vGetTime(): "));

    i2c_start_wait (DS1307_ADDR+I2C_WRITE);     // set device address and write mode
    i2c_write (DS1307_REG_SECONDS);             // write address
    i2c_rep_start (DS1307_ADDR+I2C_READ);       // set device address and read mode
    for (ucI=0; ucI<sizeof(aucData); ucI++)
    {
        aucData[ucI] = i2c_readAck();
        RTC_PRINTF_P (PSTR("%02X "), aucData[ucI]);
    }
    (void)i2c_readNak();    // one more read - unnecessary
    i2c_stop();
    RTC_PRINTF_P (PSTR("\n"));

    ptdLocalTime->tm_sec  = BCD2DEC(aucData[0] & REG_SECONDS_MASK);
    ptdLocalTime->tm_min  = BCD2DEC(aucData[1]);
    ptdLocalTime->tm_hour = BCD2DEC(aucData[2]);
}

/**
 * Sets current and RTC clock
 *
 * @param ucSec
 * @param ucMin
 * @param ucHour
 */
void RTC_vSetTime(unsigned char ucHour, unsigned char ucMin, unsigned char ucSec)
{
    RTC_PRINTF_P (PSTR("RTC_vSetTime(%02d,%02d,%02d)\n"), ucHour, ucMin, ucSec);

     i2c_start_wait (DS1307_ADDR+I2C_WRITE);     // set device address and write mode
     i2c_write (DS1307_REG_SECONDS);             // write address
     i2c_write (DEC2BCD(ucSec));
     i2c_write (DEC2BCD(ucMin));
     i2c_write (DEC2BCD(ucHour));
     i2c_stop();

     ptdLocalTime->tm_sec  = ucSec;
     ptdLocalTime->tm_min  = ucMin;
     ptdLocalTime->tm_hour = ucHour;
}
