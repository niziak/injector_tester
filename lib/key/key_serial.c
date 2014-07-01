/*
 * key_serial.c
 *
 *  Created on: Sep 30, 2013
 *      Author: nizinski_w
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <config.h>
#include <key.h>
#include <events.h>

#include <stdio.h>
#include <usart0.h>
#include <rtc.h>

typedef enum
{
    TIME_SET_NONE,
    TIME_SET_WAIT_FOR_HOUR_1,
    TIME_SET_WAIT_FOR_HOUR_2,

    TIME_SET_WAIT_FOR_MIN_1,
    TIME_SET_WAIT_FOR_MIN_2,

    TIME_SET_WAIT_FOR_SEC_1,
    TIME_SET_WAIT_FOR_SEC_2,
    TIME_SET_WAIT_FOR_END
} TIME_SET_STATE_DEF;

volatile TIME_SET_STATE_DEF eTimeState;

volatile unsigned char ucNewHour;
volatile unsigned char ucNewMin;
volatile unsigned char ucNewSec;

//#define SET_HI_NIBBLE(var,val)  { var  = ((val) & 0x0F) << 4; }
//#define SET_LO_NIBBLE(var,val)  { var |= ((val) & 0x0F)     ; }

#define SET_HI_NIBBLE(var,val)  { var  = (val) * 10; }
#define SET_LO_NIBBLE(var,val)  { var += (val)     ; }

static void vTimeSetHandler (unsigned char ucRXByte)
{
    switch (eTimeState)
    {
        default:
            eTimeState = TIME_SET_NONE;
            break;

        case TIME_SET_WAIT_FOR_HOUR_1:
            eTimeState = TIME_SET_WAIT_FOR_HOUR_2;
            SET_HI_NIBBLE (ucNewHour, ('0' - ucRXByte))
            break;

        case TIME_SET_WAIT_FOR_HOUR_2:
            eTimeState = TIME_SET_WAIT_FOR_MIN_1;
            SET_LO_NIBBLE (ucNewHour, ('0' - ucRXByte))
            break;

        case TIME_SET_WAIT_FOR_MIN_1:
            eTimeState = TIME_SET_WAIT_FOR_MIN_2;
            SET_HI_NIBBLE (ucNewMin,  ('0' - ucRXByte))
            break;

        case TIME_SET_WAIT_FOR_MIN_2:
            eTimeState = TIME_SET_WAIT_FOR_SEC_1;
            SET_LO_NIBBLE (ucNewMin,  ('0' - ucRXByte))
            break;

        case TIME_SET_WAIT_FOR_SEC_1:
            eTimeState = TIME_SET_WAIT_FOR_SEC_2;
            SET_HI_NIBBLE (ucNewSec,  ('0' - ucRXByte))
            break;

        case TIME_SET_WAIT_FOR_SEC_2:
            eTimeState = TIME_SET_WAIT_FOR_END;
            SET_LO_NIBBLE (ucNewSec,  ('0' - ucRXByte))
            break;

        case TIME_SET_WAIT_FOR_END:
            eTimeState = TIME_SET_NONE;
            if ('E' == ucRXByte)
            {
                RTC_vSetTime (ucNewHour, ucNewMin, ucNewSec);
            }
            break;
    }
}

ISR(USART_RX_vect)
{
    unsigned char ucRXByte;


    ucRXByte = USART0_ucGetByte();
    //LCD_vPrintf_P ("RX %02X ", ucRXByte);
    if (eTimeState != TIME_SET_NONE)
    {
        vTimeSetHandler (ucRXByte);
        return;
    }
    eTimeState = TIME_SET_NONE;
    switch (ucRXByte)
    {
        case ' ':  // space - special action to hang device
            for (;;) wdt_reset();
            break;

        case '\r':  // enter
            EventPostFromIRQ (MENU_ACTION_SELECT);
            break;

        case 0x41: // up arrow
            EventPostFromIRQ (MENU_ACTION_UP       );
            break;

        case 0x42: // down arrow
            EventPostFromIRQ (MENU_ACTION_DOWN     );
            break;

        case 0x44: // left
            EventPostFromIRQ (MENU_ACTION_LEFT     );
            break;

        case 0x43: // right
            EventPostFromIRQ (MENU_ACTION_RIGHT    );
            break;

        case 'T': // set time command
            eTimeState = TIME_SET_WAIT_FOR_HOUR_1;
            break;

        default:
            break;
    }
    SERIAL_KEY_PRESSED_TRIGGER_FN;
}


/**
 * Initialize Keyboard emulator on serial port
 * USART speed should be initialized previously by USART0_vInit()
 */
void KEY_SERIAL_vInit(void)
{
    UCSR0B |= (
                   (1<<RXEN0)           // Enable receiver
                 | (1<<RXCIE0)          // Enable RX complete interrupt
               );

}
