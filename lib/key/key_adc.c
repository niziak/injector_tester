/*
 * adc.c
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <config.h>
#include <key.h>
#include <events.h>

#if (USE_ARDUINO_ADC_KEYPAD)

volatile unsigned char ucADC;
volatile unsigned int uiADCKeyPressedCounter;        ///< 0 if key released. Incremented if key pressed
volatile unsigned int uiADCKeyBlockedTTL;            ///< 0 - unblocked, automatically decremented

#define WITH_INT_OVERLAP_DETECTION   FALSE           ///< ADC in non blocking mode, with nesting detected

#if WITH_INT_OVERLAP_DETECTION
static BOOL bInISR;
#endif

#define UCHAR_DISTANCE(a,b)  ( (a>b) ? (a-b) : (b-a) )

/**
 * Compare values with defined by @ref ADC_TOLERANCE tolerance
 */
static BOOL bFuzzyCompare (unsigned char a, unsigned char b)
{
    if (UCHAR_DISTANCE(a,b) < ADC_TOLERANCE)
    {
        return TRUE;
    }
    return FALSE;

}

#if WITH_INT_OVERLAP_DETECTION
ISR(ADC_vect, ISR_NOBLOCK)
#else
ISR(ADC_vect)
#endif
{
#if (WITH_INT_OVERLAP_DETECTION)
    if (bInISR==TRUE)
    {
        RESET("ADC OVLP");
    }
    bInISR = TRUE;
#endif

    ucADC = ADCH;
    if (uiADCKeyBlockedTTL>0)
    {
        uiADCKeyBlockedTTL--;
    }
    else
    {
        uiADCKeyPressedCounter++;
    }

    if (bFuzzyCompare(ucADC, ADC_KEY_VAL_IDLE))
    {
        uiADCKeyPressedCounter=0;
        // no key pressed
    }
    else if (uiADCKeyPressedCounter > ADC_KEY_DEBOUNCE * ADC_TICK_MS)
    {
        // key was pressed for enough time
        if  (TRUE == bFuzzyCompare(ucADC, ADC_KEY_VAL_SELECT))            EventPostFromIRQ(MENU_ACTION_SELECT   );
        if  (TRUE == bFuzzyCompare(ucADC, ADC_KEY_VAL_LEFT  ))            EventPostFromIRQ(MENU_ACTION_LEFT     );
        if  (TRUE == bFuzzyCompare(ucADC, ADC_KEY_VAL_DOWN  ))            EventPostFromIRQ(MENU_ACTION_DOWN     );
        if  (TRUE == bFuzzyCompare(ucADC, ADC_KEY_VAL_UP    ))            EventPostFromIRQ(MENU_ACTION_UP       );
        if  (TRUE == bFuzzyCompare(ucADC, ADC_KEY_VAL_RIGHT ))            EventPostFromIRQ(MENU_ACTION_RIGHT    );

        ADC_KEY_PRESSED_TRIGGER_FN
        uiADCKeyBlockedTTL = ADC_KEY_INTERVAL * ADC_TICK_MS;
        uiADCKeyPressedCounter = 0;
    }


#if (WITH_INT_OVERLAP_DETECTION)
    bInISR = FALSE;
#endif
    //ADCSRA |= 1<<ADSC; // start conversion - single mode
}


/**
 * Initialize ADC in free running mode
 */
void KEY_ADC_vInit(void)
{
    ADMUX = (   (1<<ADLAR)                            // ADC Left adjust
              | (1<<REFS0)                 // 00 - ext AREF (int turned off), 01 - Vcc with capacitor, 11 - internal 1,1 reference with capacitor
/*              | (1<<MUX3)  | (1<<MUX2)  | (1<<MUX1)       // 1110 - 1,30V Vbg     0000-ADC0*/
            );

#if defined (__AVR_ATmega8__)
    ADCSRA = (   (1<<ADFR)             // free running mode
               | (1<<ADIE)             // ADC int enable
               | (1<<ADPS2)            // prescaler 111 - divide by 128
               | (1<<ADPS1)
               | (1<<ADPS0)
             );

#elif defined (__AVR_ATmega328P__)

    ADCSRA = (   (1<<ADATE)            // auto trigger enable
               | (1<<ADIE)             // ADC int enable
               | (1<<ADPS2)            // prescaler 111 - divide by 128
               | (1<<ADPS1)
               | (1<<ADPS0)
             );
//    ADCSRB = (                        // 0 = free running mode
//
//             );
      DIDR0 = (
                  (1<<ADC0D)                 // digital PIN for ADC0 disabled (PC0)
              );
#else
    #error "CPU!"
#endif


    ADCSRA |= (   (1<<ADEN)                // ADC enable
                | (1<<ADSC)                // start first conversion
              );

}

#endif //(USE_ARDUINO_ADC_KEYPAD)

