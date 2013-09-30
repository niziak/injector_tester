/*
 * rtc.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */
#include "config.h"
#include "rtc.h"
#include <string.h>
#include <util/atomic.h>
#include <i2cmaster.h>
#include <stdio.h>

//#define DS1307_ADDR            0x68    // 104=0x68
/**
 * !!! i2cmaster is using 8 bit addresses, so shift your addres left
 */
#define DS1307_ADDR            0xD0    // 1101000R = 0xD0
#define DS1307_REG_SECONDS     0
    #define REG_SECONDS_CLOCK_HALT  7
#define DS1307_REG_MINUTES     1
#define DS1307_REG_HOURS       2
#define DS1307_REG_WDAY        3
#define DS1307_REG_DAY         4
#define DS1307_REG_MONTH       5
#define DS1307_REG_YEAR        6
#define DS1307_REG_CONTROL     7

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


tm tdLocalTime;
time_t  tSecondsUntilEpoch;
time_t  tOffsetFrom1970;

void RTC_vInit(void)
{
    memset (&tdLocalTime,0, sizeof(tdLocalTime));
    tSecondsUntilEpoch = 0;

    DDRC   &= ~_BV(PINC5);      // input
    PORTC |=   _BV(PINC5);      // pull up

    DDRC   &= ~_BV(PINC4);      // input
    PORTC |=   _BV(PINC4);      // pull up


    i2c_init();                             // initialize I2C library

////    i2c_start_wait(DS1307_ADDR+I2C_WRITE);     // set device address and write mode
////    i2c_write(DS1307_REG_SECONDS);                        // write address = 5
////    i2c_write(0x00);
////    i2c_write(0x00);
////    i2c_write(0x14);
////    i2c_stop();
//
//    while (1)
//    {
//        printf("\n\nI2C:\n");
//
//        i2c_start_wait(DS1307_ADDR+I2C_WRITE);     // set device address and write mode
//        printf("write=%d\n", i2c_write(DS1307_REG_SECONDS));             // write address
//        printf("start=%d\n", i2c_rep_start(DS1307_ADDR+I2C_READ));       // set device address and read mode
//        printf("sec=%02X\n", i2c_readAck());
//        printf("min=%02X\n", i2c_readAck());
//        printf("hour=%02X\n", i2c_readNak());
//        i2c_stop();
//
////        printf("start=%d\n", i2c_start(DS1307_ADDR+I2C_WRITE));       // set device address and read mode
////        i2c_stop();
//
////        i2c_start_wait(DS1307_ADDR+I2C_WRITE);     // set device address and write mode
////        i2c_write(DS1307_RAM_START);                        // write address = 5
////        i2c_write(0x75);                        // write value 0x75 to EEPROM
////        i2c_stop();
////
////        printf("I2C written.\n");
////
////        i2c_start_wait(DS1307_ADDR+I2C_WRITE);     // set device address and write mode
////        printf("write=%d\n", i2c_write(DS1307_RAM_START));             // write address
////        printf("start=%d\n", i2c_rep_start(DS1307_ADDR+I2C_READ));       // set device address and read mode
////
////        printf("Value=%02X\n", i2c_readNak());                    // read one byte from EEPROM
////
////        i2c_stop();
////        printf("I2C read back.\n");
//
//        _delay_ms(1000);
//    }
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


void RTC_vShowTime(void)
{
    printf("\n\nI2C:\n");

    i2c_start_wait(DS1307_ADDR+I2C_WRITE);     // set device address and write mode
    printf("write=%d\n", i2c_write(DS1307_REG_SECONDS));             // write address
    printf("start=%d\n", i2c_rep_start(DS1307_ADDR+I2C_READ));       // set device address and read mode
    printf("sec=%02X\n", i2c_readAck());
    printf("min=%02X\n", i2c_readAck());
    printf("hour=%02X\n", i2c_readNak());
    i2c_stop();
}
