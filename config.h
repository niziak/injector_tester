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



#define HB_LED_PORT     PORTB
#define HB_LED_DDR      DDRB
#define HB_LED_PIN      PINB5

#define HB_LED_ALTER        {   HB_LED_PORT ^=  _BV(HB_LED_PIN); }
#define HB_LED_LOW          {   HB_LED_PORT &=~ _BV(HB_LED_PIN); }
#define HB_LED_HI           {   HB_LED_PORT |=  _BV(HB_LED_PIN); }
#define HB_LED_SETUP        {   HB_LED_DDR  |=  _BV(HB_LED_PIN); }


#define LCD_BL_PORT    PORTB
#define LCD_BL_DDR     DDRB
#define LCD_BL_PIN     PINB2

#define LCD_BL_ALTER   {   LCD_BL_PORT ^=  _BV(LCD_BL_PIN); }
#define LCD_BL_LOW     {   LCD_BL_PORT &=~ _BV(LCD_BL_PIN); }
#define LCD_BL_HI      {   LCD_BL_PORT |=  _BV(LCD_BL_PIN); }
#define LCD_BL_SETUP   {   LCD_BL_DDR  |=  _BV(LCD_BL_PIN); }




#define INJECTOR_TESTER_MODE	0

#define ONEWIRE_ZASO_IDX	0
#define ONEWIRE_KRAN_IDX		1

#define UI_INACTIVE_TIMEOUT     60          ///< (in seconds) go to idle mode when no keypresse in 60 seconds

#define AVRLIB_HAS_EEPROM_UPDATE_BLOCK_FN       FALSE


extern volatile unsigned long ulSystemTickMS;
extern volatile unsigned long ulSystemTickS;
extern volatile BOOL bBlinkState;
extern volatile UCHAR ucUIInactiveCounter;
extern volatile unsigned int uiPumpSwitchOffAfter;
extern volatile BOOL bRefreshDisplay;

#endif /* CONFIG_H_ */
