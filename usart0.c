
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include <config.h>
#include <usart0.h>

static FILE USART0_stream = FDEV_SETUP_STREAM (USART0_iSendByteToStream, NULL, _FDEV_SETUP_WRITE);

void USART0_vInit(void)
{
    // Set baud rate
    UBRR0H = (unsigned char) (USART0_UBBR_VAL >>8 );
    UBRR0L = (unsigned char) (USART0_UBBR_VAL);

    // Enable receiver and transmitter
    UCSR0B = (
                 (1<<RXEN0)
               | (1<<TXEN0)
             );

    // set asynchronous mode, 8 bit data, NO parity, 1 bit stop
    UCSR0C = (
                 (1<<UCSZ01) | (1<<UCSZ00)      // 8 bit
             );

    stderr = stdout = &USART0_stream;
}

/**
 * Transmit single character
 * @param ucByte
 */
void USART0_vSendByte (unsigned char ucByte)
{
    // wait for data registry empty
    while ( ! (UCSR0A & (1<<UDRE0))) { };
    UDR0 = ucByte;
}

int USART0_iSendByteToStream (unsigned char ucByte, FILE *stream)
{
    // implicit CR on every NL
    if (ucByte == '\n')
    {
        USART0_vSendByte ('\r');
    }
    USART0_vSendByte (ucByte);
    return 1;
}

/**
 * Blocking!
 * @return
 */
unsigned char USART0_ucGetByte(void)
{
    // wait for data
    while ( ! (UCSR0A & (1<<RXC0))) { };
    return UDR0;
}

