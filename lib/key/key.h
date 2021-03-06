/*
 * key.h
 *
 *  Created on: Sep 23, 2013
 *      Author: nizinski_w
 */

#ifndef KEY_H_
#define KEY_H_

#define USE_SERIAL_EMULATED_KEYPAD      TRUE
#define USE_ARDUINO_ADC_KEYPAD          TRUE
#define USE_SINGLE_PIN_BUTTONS          FALSE

#define KEY_USE_TIMER_TICK              FALSE
    #define KEY_TIMER_TICK_EVERY_MS         10


#if (USE_ARDUINO_ADC_KEYPAD)

    #include <key_adc.h>
    #define KEY_vInit(a)        KEY_ADC_vInit()

    #define  ADC_TOLERANCE          5
    // must be from low to high
    #define  ADC_KEY_VAL_RIGHT      0
    #define  ADC_KEY_VAL_UP         24
    #define  ADC_KEY_VAL_DOWN       64
    #define  ADC_KEY_VAL_LEFT       102
    #define  ADC_KEY_VAL_SELECT     160

    #define  ADC_KEY_VAL_IDLE       255

    // ADC prescaler is /128, with 16MHz clock, gives 16000/128 = 125kHz, 8 uS / tick
    // Conversion in auto triggered mode takes ~13,5 ticks, so ADC INT comes every 108 uS = 0,1 ms
    #define ADC_TICK_MS             11             ///< (us) how many ADC tick takes 1 ms
    #define ADC_KEY_DEBOUNCE        50             ///< n* ms     how many cycles wait for key unchanged state
    #define ADC_KEY_INTERVAL        200            ///< n* ms    how long block keyboard reading after keypress (key repeat)

    #include <tools.h>
    #define ADC_KEY_PRESSED_TRIGGER_FN      {breakable_delay_break();}


#elif (USE_SINGLE_PIN_BUTTONS)

    #define KEY_DEBOUNCE        2       ///< n*10ms     how many cycles wait for key unchanged state
    #define KEY_INTERVAL        5       ///< n*10ms     how long block keyboard reading after keypress

    #include <key_buttons.h>


#endif

#if (USE_SERIAL_EMULATED_KEYPAD)
    #include <key_serial.h>
    #define SERIAL_KEY_PRESSED_TRIGGER_FN              {breakable_delay_break();}
#endif

#endif /* KEY_H_ */
