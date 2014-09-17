/*
 * dcf77.h
 *
 *  Created on: 9 wrz 2014
 *      Author: nizinski_w
 */

#ifndef DCF77_H_
#define DCF77_H_

#define DCF77_DEBUG_P(format, args...)          { DEBUG_P(format, ##args); }

#define DCF77_PINS      PINC
#define DCF77_PIN_DATA  PINC1
#define DCF77_PORT      PORTC
#define DCF77_PORT_DDR  DDRC

#define DCF77_PIN_GET         ( bit_is_set(DCF77_PINS, DCF77_PIN_DATA) ? 1 : 0 )
#define DCF77_PORT_SETUP      {   DCF77_PORT_DDR &= ~_BV(DCF77_PIN_DATA); DCF77_PORT |= _BV(DCF77_PIN_DATA); }        // DDR=0 - input port, PORT=1 - pull up





#define bit_test(data,bit)    ((data) & _BV(bit))

typedef     unsigned char   bool;
typedef     unsigned char   uint8;
typedef     unsigned int    uint16;

extern bool dcf77_get_sync(void);
extern uint8 dcf77_get_min(void);
extern uint8 dcf77_get_hrs(void);
extern uint8 dcf77_get_day(void);
extern uint8 dcf77_get_dow(void);
extern uint8 dcf77_get_month(void);
extern uint16 dcf77_get_year(void);
extern bool dcf77_newdata(void);




typedef enum
{
    DS_WAIT_FOR_NEXT_FRAME,  ///< ignore all pulses, and wait for gap which is indicating beginning of next frame
    DS_WAIT_FOR_WEATHER,
} DCF_STATE_DEF;

typedef struct
{
    DCF_STATE_DEF   eDCFState;
} DCF_DECODER;


#define DCF_TASK_EVERY_US   1000    ///< DCFTask() is called every xxx US
#define DCF_GET_PIN     0           ///< function for sampling PIN

extern void DCFTask(void);


#endif /* DCF77_H_ */
