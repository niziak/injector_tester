/*
 * nvm.c
 *
 *  Created on: Sep 12, 2013
 *      Author: nizinski_w
 */

#include <stdint.h>
#include <avr/eeprom.h>
#include "nvm.h"
#include <1wire_config.h>
#include <string.h>

#define NVM_MAGIC_LEN       5
#define NVM_MAGIC_BYTE      0xA5
#define NVM_VERSION         1

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
    BOOL bError = FALSE;
    unsigned char aucMagic[NVM_MAGIC_LEN];
    unsigned int  uiVersion;

    eeprom_read_block ( &(aucMagic[0]), &(NVM_aucMagic[0]), sizeof(aucMagic) );
    eeprom_read_block ( &(uiVersion),                 &(NVM_uiVersion),                sizeof(uiVersion)          );

    for (UCHAR a=0; a<NVM_MAGIC_LEN; a++)
    {
        if (aucMagic[a] != NVM_MAGIC_BYTE)
        {
            bError = TRUE;
        }
    }

    if (uiVersion != NVM_VERSION)
    {
        bError = TRUE;
    }

    if (bError == TRUE)
    {
        LOG("No NVM\n");
        return;
    }
    eeprom_read_block ( &(atdKnownTempSensors[0]),    &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
}

void NVM_vSaveSettings(void)
{
    unsigned char aucMagic[NVM_MAGIC_LEN];
    unsigned int uiVersion = NVM_VERSION;
    memset (&aucMagic[0], NVM_MAGIC_BYTE, sizeof (aucMagic));
#if (AVRLIB_HAS_EEPROM_UPDATE_BLOCK_FN)
    eeprom_update_block ( &(uiVersion),               &(NVM_uiVersion),                sizeof(uiVersion)          );
    eeprom_update_block ( &(aucMagic[0]),             &(NVM_aucMagic[0]),              sizeof(aucMagic)           );
    eeprom_update_block ( &(atdKnownTempSensors[0]),  &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
#else
    eeprom_write_block ( &(uiVersion),               &(NVM_uiVersion),                sizeof(uiVersion)          );
    eeprom_write_block ( &(aucMagic[0]),             &(NVM_aucMagic[0]),              sizeof(aucMagic)           );
    eeprom_write_block ( &(atdKnownTempSensors[0]),  &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
#endif
}
