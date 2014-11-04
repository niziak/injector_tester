/*
 * config.h
 *
 *  Created on: Aug 9, 2013
 *      Author: nizinski_w
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <types.h>
#include <util/delay.h>

#define PIR_PORT          PORTD
#define PIR_DDR           DDRD
#define PIR_PIN           PIND2
#define PIR_PINS          PIND

#define PIR_SETUP             {   PIR_DDR &= ~_BV(PIR_PIN); PIR_PORT |= _BV(PIR_PIN); }        // DDR=0 - input port, PORT=1 - pull up



#define PUMP_LED_PORT     PORTC
#define PUMP_LED_DDR      DDRC
#define PUMP_LED_PIN      PINC2

#define PUMP_LED_ALTER        {   PUMP_LED_PORT ^=  _BV(PUMP_LED_PIN); }
#define PUMP_LED_LO           {   PUMP_LED_PORT &=~ _BV(PUMP_LED_PIN); }
#define PUMP_LED_HI           {   PUMP_LED_PORT |=  _BV(PUMP_LED_PIN); }
#define PUMP_LED_SETUP        {   PUMP_LED_DDR  |=  _BV(PUMP_LED_PIN); }



#define HB_LED_PORT     PORTB
#define HB_LED_DDR      DDRB
#define HB_LED_PIN      PINB5

#define HB_LED_ALTER        {   HB_LED_PORT ^=  _BV(HB_LED_PIN); }
#define HB_LED_LO           {   HB_LED_PORT &=~ _BV(HB_LED_PIN); }
#define HB_LED_HI           {   HB_LED_PORT |=  _BV(HB_LED_PIN); }
#define HB_LED_SETUP        {   HB_LED_DDR  |=  _BV(HB_LED_PIN); }


/**
 * Arduino LCD shield has NPN transistor with 4,7 kOhm pull-up resistor, which is responsible for keeping backlight on all time.
 * To switch backlight off, base of transistor (connected to D10 = PINB2) needs to be pulled-down to ground.
 * But, when D10=PINB2 goes to HI state, very high current is flowing between D10 and BE junction of transistor.
 *
 * To prevent this, port will be switched to input mode instead to go to HI. Additional 4,7 kOhm resistor will be responsible for turning LCD on.
 *
 * so simply - set port state to LOW, and then swithc only DDR from output to input
 * TODO - cut track to D10 and put diode
 */
#define LCD_BL_PORT    PORTB
#define LCD_BL_DDR     DDRB
#define LCD_BL_PIN     PINB2

#define LCD_BL_ALTER   {   LCD_BL_DDR  ^=  _BV(LCD_BL_PIN); }

#define LCD_BL_LO      {   LCD_BL_DDR  |=  _BV(LCD_BL_PIN); }
#define LCD_BL_HI      {   LCD_BL_DDR  &= ~_BV(LCD_BL_PIN); }
#define LCD_BL_SETUP   {   LCD_BL_PORT &= ~_BV(LCD_BL_PIN); }



#define WITH_HB_EVENT           0

#define INJECTOR_TESTER_MODE    0

#define ONEWIRE_ZASO_IDX        0
#define ONEWIRE_KRAN_IDX        1

#define ONEWIRE_MEASURE_INTERVAL_MS         1000        ///< after reading, wait this time before next conversion
#define ONEWIRE_MEASURE_WAIT_MS             2000        ///< delay between start conversion command and reading temperature form sensor

#define UI_INACTIVE_TIMEOUT                 20          ///< (in seconds) go to idle mode when no keypressed (also lcd backlight time)
#define BLINK_SPEED_MS                      400         ///< (in ms)

#define UI_NEG_POPUP_TIME                   4000
#define UI_POS_POPUP_TIME                   3000

#define DEFAULT_SEC_PER_DAY_ADJ            -8
    #define SEC_PER_DAY_ADJ_MIN            -20
    #define SEC_PER_DAY_ADJ_MAX             20         ///< maximum daily time adjustment (because of limits in @ref RTC_vCheckForDailyAdjustment)
#define DEFAULT_ZASO_MIN_TEMP               38
#define DEFAULT_KRAN_MIN_TEMP               35
#define DEFAULT_KRAN_MAX_TEMP               40
#define DEFAULT_PUMP_PIR_MAX_TTL            300         ///< (in seconds) how long pump can be on after PIR event (or to reach maximum temperature)
#define DEFAULT_PUMP_MANUAL_TTL             (10*60)     ///< (in seconds) how long pump should work when turned on from menu

#define AVRLIB_HAS_EEPROM_UPDATE_BLOCK_FN       FALSE

#define LCD_CHAR_CELSIUS                    ((unsigned char)223)

#define NUMBER_OF_USER_MODES                4           ///<
#define TIMERANGES_PER_USER_MODE            5           ///< how many time ranges can be defined per user mode

#define WITH_RTC_DRIFT_MEASURE  0               ///< use system timer to check RTC drift - not usable because quartz 16MHz is also not well calibrated :)
    #define RTC_OFFSET_MEASURE_TIME_MSEC        (6*3600UL*1000UL)         ///< how long period is taken to calculate RTC drift

#define TIMER0_ISR_EVERY_US                 (1000)      ///< Timer ISR is called every ??? us

#define WITH_DCF77_SUPPORT                  TRUE

#include <globals.h>
#include <log.h>


#endif /* CONFIG_H_ */
