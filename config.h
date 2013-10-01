/*
 * config.h
 *
 *  Created on: Aug 9, 2013
 *      Author: nizinski_w
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <types.h>
#include <log.h>
#include <util/delay.h>


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


#define LCD_BL_PORT    PORTB
#define LCD_BL_DDR     DDRB
#define LCD_BL_PIN     PINB2

#define LCD_BL_ALTER   {   LCD_BL_PORT ^=  _BV(LCD_BL_PIN); }
#define LCD_BL_LO      {   LCD_BL_PORT &=~ _BV(LCD_BL_PIN); }
#define LCD_BL_HI      {   LCD_BL_PORT |=  _BV(LCD_BL_PIN); }
#define LCD_BL_SETUP   {   LCD_BL_DDR  |=  _BV(LCD_BL_PIN); }



#define WITH_HB_EVENT           0

#define INJECTOR_TESTER_MODE    0

#define ONEWIRE_ZASO_IDX        0
#define ONEWIRE_KRAN_IDX        1

#define ONEWIRE_MEASURE_INTERVAL_MS         10000
#define ONEWIRE_MEASURE_WAIT_MS             2000

#define UI_INACTIVE_TIMEOUT                 20          ///< (in seconds) go to idle mode when no keypresse in 60 seconds
#define BLINK_SPEED                         400         ///< (in ms)

#define AVRLIB_HAS_EEPROM_UPDATE_BLOCK_FN       FALSE


extern volatile unsigned long       ulSystemTickMS;
extern volatile unsigned long       ulSystemTickS;
extern volatile BOOL                bBlinkState;
extern volatile UCHAR               ucUIInactiveCounter;
extern volatile unsigned int        uiPumpSwitchOffAfter;
extern volatile BOOL                bRefreshDisplay;
extern volatile BOOL                bNeedsBlinking;

#define DISP_REFRESH                {bRefreshDisplay = TRUE;}
#define DISP_START_BLINK_TIMER      {bNeedsBlinking = TRUE;}
#define DISP_STOP_BLINK_TIMER       {bNeedsBlinking = FALSE;}

#endif /* CONFIG_H_ */
