/*
 * config.h
 *
 *  Created on: Aug 9, 2013
 *      Author: nizinski_w
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <types.h>
#include <main.h>
#include <log.h>
#include <util/delay.h>

#define PIR_PORT          PORTD
#define PIR_DDR           DDRD
#define PIR_PIN           PIND2
#define PIR_PINS          PIND

#define PIR_SETUP             {   PIR_DDR &= ~_BV(PIR_PIN); PIR_PORT |= _BV(PIR_PIN); }        // DDR=0 - input port, PORT=1 - pull up


#define PUMP_LED_PORT     PORTC
#define PUMP_LED_DDR      DDRC
#define PUMP_LED_PIN      PINB2

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

#define ONEWIRE_MEASURE_INTERVAL_MS         10000
#define ONEWIRE_MEASURE_WAIT_MS             2000

#define UI_INACTIVE_TIMEOUT                 20          ///< (in seconds) go to idle mode when no keypressed (also lcd backlight time)
#define BLINK_SPEED                         400         ///< (in ms)

#define UI_NEG_POPUP_TIME                   4000
#define UI_POS_POPUP_TIME                   3000
#define PIR_PRESENCE_TTL                    60          ///< (in seconds) how long presence is reported after it was detected
#define PUMP_MANUAL_TTL                     (10*60)     ///< (in seconds) how long pump should work when turned on from menu

#define AVRLIB_HAS_EEPROM_UPDATE_BLOCK_FN       FALSE



#endif /* CONFIG_H_ */
