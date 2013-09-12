/*
 * nvm.c
 *
 *  Created on: Sep 12, 2013
 *      Author: nizinski_w
 */

#include <stdint.h>
#include <avr/eeprom.h>
#include "nvm.h"
#include <lib/1wire_config.h>
#include <string.h>

#define NVM_MAGIC_LEN       5
#define NVM_MAGIC_BYTE      0xA5

static EEMEM unsigned char                  NVM_aucMagic[NVM_MAGIC_LEN];
static EEMEM unsigned int                   NVM_uiVersion;
static EEMEM TEMP_SENSOR_PARAMS_DEF         NVM_atdKnownTempSensors[NUM_OF_TEMP_SENSORS];

void NVM_vRestoreFactory(void)
{
    unsigned char aucMagic[NVM_MAGIC_LEN];
    memset (&aucMagic[0], 0x0, sizeof (aucMagic));
    eeprom_write_block ( &(aucMagic[0]), &(NVM_aucMagic[0]), sizeof(aucMagic) );
}

void NVM_vLoadSettings(void)
{
    unsigned char aucMagic[NVM_MAGIC_LEN];
    eeprom_read_block ( &(aucMagic[0]), &(NVM_aucMagic[0]), sizeof(aucMagic) );

    for (UCHAR a=0; a<NVM_MAGIC_LEN; a++)
    {
        if (aucMagic[a] != NVM_MAGIC_BYTE)
        {
            LOG("No NVM");
            return;
        }
    }

    eeprom_read_block ( &(atdKnownTempSensors[0]),    &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
}

void NVM_vSaveSettings(void)
{
    unsigned char aucMagic[NVM_MAGIC_LEN];
    memset (&aucMagic[0], NVM_MAGIC_BYTE, sizeof (aucMagic));
    eeprom_update_block ( &(aucMagic[0]),             &(NVM_aucMagic[0]),              sizeof(aucMagic) );
    eeprom_update_block ( &(atdKnownTempSensors[0]),  &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
}
