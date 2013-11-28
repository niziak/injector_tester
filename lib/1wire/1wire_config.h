/*
 * 1wire_config.h
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef OWIRE_CONFIG_H_
#define OWIRE_CONFIG_H_



#define NUM_OF_TEMP_SENSORS		        2
#define OW_ADDRESS_LEN			        8
#define TEMP_SENSOR_NAME_LEN	        8
#define OW_WITH_SENSOR_NAMES            FALSE

#include <avr/io.h>
#include <util/delay.h>
#include <types.h>
#include <config.h>

#define  ALIGN2

#define OW_PORT		PORTC
#define OW_PINS		PINC
#define OW_DDR		DDRC
#define OW_PIN		PINC3

#define OW_1    {OW_PORT |=  _BV(OW_PIN);  }
#define OW_0    {OW_PORT &=~ _BV(OW_PIN);  }
#define OW_out  {OW_DDR  |=  _BV(OW_PIN);  }
#define OW_in   {OW_DDR  &= ~_BV(OW_PIN);  }
#define OW_get  ( bit_is_set(OW_PINS, OW_PIN) ? 1 : 0 )

#include <avr/interrupt.h>
#define OW_CRITICAL_ENTER           {cli();}  //TODO
#define OW_MICRO_DELAY(a)			_delay_us(a)
#define OW_CRITICAL_EXIT            {sei();}  //TODO in some cases better to restore previous int state

#define OW_GCC_OPT2
//#define OW_GCC_OPT2 __attribute__((optimize(2)))

#define OW_SLEEP_MS(a)					_delay_ms(a)
//#define OW_SLEEP_MS(a)					NutSleep(a)

#include <stdio.h>
#include <avr/pgmspace.h>
#define PRINTF(a...)                    printf(a)
#define PRINTF_P(a...)                  printf_P(a)
#define PRINTF_T(a...)                  printf(a)
#define PRINTF_T_P(...)                 printf_P(a)



typedef struct
{
  UCHAR aucROM[OW_ADDRESS_LEN];           ///< unique 1W sensor address
} OW_NEW_DEVICE_DEF ;

typedef struct PACKED
{
  UCHAR aucROM[OW_ADDRESS_LEN];           ///< unique 1W sensor address
  INT   iTempInt ALIGN2;                  ///< current sensor temperature (integer part)
  INT   iTempFrac ALIGN2;                 ///< current sensor temperature (fractional part)
  CHAR  cTempAdj;                         ///< temp adjustment for sensor
  UCHAR ucStatus;                         ///< 1 if sensor was found in last scan
#if (OW_WITH_SENSOR_NAMES)
  CHAR  acName[TEMP_SENSOR_NAME_LEN+1];   ///< user friendly name of sensor
#endif
} TEMP_SENSOR_PARAMS_DEF;

#include <limits.h>
#define OW_TEMP_ERROR           ((INT)SHRT_MIN) // 0x8000 //(-32767) //SHRT_MIN
#define TEMP_ERROR              (OW_TEMP_ERROR)



#if 1 // AVR

extern OW_NEW_DEVICE_DEF			atdNewTempSensors[NUM_OF_TEMP_SENSORS];
extern TEMP_SENSOR_PARAMS_DEF		atdKnownTempSensors[NUM_OF_TEMP_SENSORS];

#define OW_KNOWN_TEMP_SENSOR_ROM(ucSensIdx, index)			atdKnownTempSensors[ucSensIdx].aucROM[index]
#define OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx)			atdKnownTempSensors[ucSensIdx].iTempInt
#define OW_KNOWN_TEMP_SENSOR_TEMP_FRAC(sensoridx)			atdKnownTempSensors[ucSensIdx].iTempFrac
#define OW_KNOWN_TEMP_SENSOR_TEMP_ADJ(sensoridx)			atdKnownTempSensors[ucSensIdx].cTempAdj
#define OW_KNOWN_TEMP_SENSOR_STATUS(ucSensIdx)				atdKnownTempSensors[ucSensIdx].ucStatus
#define OW_KNOWN_TEMP_SENSOR_NAME(ucSensIdx)                atdKnownTempSensors[ucSensIdx].acName
#define OW_NEW_DEVICE_ARRAY(num)							atdNewTempSensors[num]
#define OW_NEW_DEVICE_ARRAY_SIZE							sizeof(atdNewTempSensors)
#define OW_NEW_DEVICE_ARRAY_ELEM(num,idx)					atdNewTempSensors[num].aucROM[idx]
#define OW_NEW_DEVICE_ARRAY_ELEM_SIZE(num)					sizeof(atdNewTempSensors[num].aucROM)


#else // NutOS

#define OW_GET_KNOWN_TEMP_SENSOR_ROM(ucSensIdx, index)		stSettings.atdTempSensor[ucSensIdx].aucROM[index]
#define OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx)			stSettings.atdTempSensor[ucSensIdx].iTempInt
#define OW_KNOWN_TEMP_SENSOR_TEMP_FRAC(sensoridx)			stSettings.atdTempSensor[ucSensIdx].iTempFrac
#define OW_KNOWN_TEMP_SENSOR_TEMP_ADJ(ucSensIdx)			stSettings.atdTempSensor[ucSensIdx].cTempAdj
#define OW_KNOWN_TEMP_SENSOR_STATUS(ucSensIdx)				stSettings.atdTempSensor[ucSensIdx].ucStatus
#define OW_KNOWN_TEMP_SENSOR_NAME(ucSensIdx)                stSettings.atdTempSensor[ucSensIdx].acName
#define OW_NEW_DEVICE_ARRAY(num)							atdOWNewDevices[num]
#define OW_NEW_DEVICE_ARRAY_SIZE							sizeof(atdOWNewDevices)
#define OW_NEW_DEVICE_ARRAY_ELEM(num,idx)					atdOWNewDevices[num].aucROM[idx]
#define OW_NEW_DEVICE_ARRAY_ELEM_SIZE(num)					sizeof(atdOWNewDevices[num].aucROM)

#endif





#endif /* OWIRE_CONFIG_H_ */
