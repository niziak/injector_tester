/*
 * adc.c
 *
 *  Created on: Sep 19, 2013
 *      Author: nizinski_w
 */

#include <avr/io.h>
#include <avr/interrupt.h>

/*
 *
#define ADCW    _SFR_IO16(0x04)
#ifndef __ASSEMBLER__
#define ADC _SFR_IO16(0x04)
#endif
#define ADCL    _SFR_IO8(0x04)
#define ADCH    _SFR_IO8(0x05)
#define ADCSR   _SFR_IO8(0x06)
#define ADCSRA  _SFR_IO8(0x06)
#define ADMUX   _SFR_IO8(0x07)
 *
 */

/*
 *  ADC Conversion Complete
#define ADC_vect_num        14
#define ADC_vect            _VECTOR(14)
#define SIG_ADC             _VECTOR(14)
 *
 */

 /*
  *  ACSR
#define ACD 7
#define ACBG    6
#define ACO 5
#define ACI 4
#define ACIE    3
#define ACIC    2
#define ACIS1   1
#define ACIS0   0

 ADCSR / ADCSRA
#define ADEN    7
#define ADSC    6
#define ADFR    5
#define ADIF    4
#define ADIE    3
#define ADPS2   2
#define ADPS1   1
#define ADPS0   0

 ADMUX
#define REFS1   7
#define REFS0   6
#define ADLAR   5
 bit 4 reserved
#define MUX3    3
#define MUX2    2
#define MUX1    1
#define MUX0    0
  */

volatile unsigned int uiADC;

#define WITH_INT_OVERLAP_DETECTION   TRUE
#if WITH_INT_OVERLAP_DETECTION
static BOOL bInISR;
#endif

ISR(ADC_vect, ISR_NOBLOCK)
{
#if (WITH_INT_OVERLAP_DETECTION)
    if (bInISR==TRUE)
    {
        RESET("ADC OVLP");
    }
    bInISR = TRUE;
#endif

    uiADC = ADCH;

#if (WITH_INT_OVERLAP_DETECTION)
    bInISR = FALSE;
#endif
    //ADCSRA |= 1<<ADSC; // start conversion - singel mode
}



void ADC_vInit(void)
{
    ADMUX = (   (1<<ADLAR)                            // ADC Left adjust
              | (1<<REFS0) | (1<<REFS1)                 // REFS0 REFS1 - internal 2,56 reference
              /*| (1<<MUX3)  | (1<<MUX2)  | (1<<MUX2) */      // 1110 - 1,30V Vbg     0000-ADC0
            );

    ADCSRA = (   (1<<ADFR)             // free running mode
               | (1<<ADIE)             // ADC int enable
               | (1<<ADPS2)            // prescaler 111 - divide by 128
               | (1<<ADPS1)
               | (1<<ADPS0)
             );

    ADCSRA |= (   (1<<ADEN)                // ADC enable
                | (1<<ADSC)                // start first conversion
              );

}
