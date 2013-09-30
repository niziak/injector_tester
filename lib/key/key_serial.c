/*
 * key_serial.c
 *
 *  Created on: Sep 30, 2013
 *      Author: nizinski_w
 */

#include <avr/io.h>
#include <avr/interrupt.h>


#include <config.h>
#include <key.h>
#include <events.h>

#include <stdio.h>
#include <usart0.h>

ISR(USART_RX_vect)
{
    unsigned char ucRXByte;


    ucRXByte = USART0_ucGetByte();
    //LCD_vPrintf_P ("RX %02X ", ucRXByte);
    switch (ucRXByte)
    {
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
