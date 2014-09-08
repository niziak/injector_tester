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
#define NVM_VERSION         2

static EEMEM unsigned char                  NVM_aucMagic[NVM_MAGIC_LEN];
static EEMEM unsigned int                   NVM_uiVersion;
static EEMEM TEMP_SENSOR_PARAMS_DEF         NVM_atdKnownTempSensors[NUM_OF_TEMP_SENSORS];
static EEMEM NVM_SET_DEF                    NVM_stSettings;

void NVM_vRestoreFactory(void)
{
    unsigned char aucMagic[NVM_MAGIC_LEN];
    memset (&aucMagic[0], 0x0, sizeof (aucMagic));
    eeprom_write_block ( &(aucMagic[0]), &(NVM_aucMagic[0]), sizeof(aucMagic) );
}

/**
 * Load EEPROM to NVM
 */
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

    pstSettings->ucMinTempZasobnik  = DEFAULT_ZASO_MIN_TEMP;
    pstSettings->ucMinTempKran      = DEFAULT_KRAN_MIN_TEMP;
    pstSettings->ucMaxTempKran      = DEFAULT_KRAN_MAX_TEMP;
    pstSettings->uiPumpManualTime   = DEFAULT_PUMP_MANUAL_TTL;
    pstSettings->uiPumpPIRTime      = DEFAULT_PUMP_PIR_MAX_TTL;
    pstSettings->cSecondsPerDayAdj  = DEFAULT_SEC_PER_DAY_ADJ;
    pstSettings->uiBacklightTime    = UI_INACTIVE_TIMEOUT;

    if (bError == TRUE)
    {
        LOG("No NVM\n");
        return;
    }
    eeprom_read_block ( &(atdKnownTempSensors[0]),    &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
    eeprom_read_block ( pstSettings,                  &(NVM_stSettings),               sizeof(stSettings));

    DEBUG_MEM( &(atdKnownTempSensors[0]),    sizeof(atdKnownTempSensors));
    DEBUG_MEM( pstSettings,                  sizeof(stSettings));
}

/**
 * Save NVM to EEPROM block
 */
void NVM_vSaveSettings(void)
{
    unsigned char aucMagic[NVM_MAGIC_LEN];
    unsigned int uiVersion = NVM_VERSION;

    DEBUG_MEM( &(atdKnownTempSensors[0]),    sizeof(atdKnownTempSensors));
    DEBUG_MEM( pstSettings,                  sizeof(stSettings));

    memset (&aucMagic[0], NVM_MAGIC_BYTE, sizeof (aucMagic));
#if (AVRLIB_HAS_EEPROM_UPDATE_BLOCK_FN)
    eeprom_update_block ( &(uiVersion),               &(NVM_uiVersion),                sizeof(uiVersion)          );
    eeprom_update_block ( &(aucMagic[0]),             &(NVM_aucMagic[0]),              sizeof(aucMagic)           );
    eeprom_update_block ( &(atdKnownTempSensors[0]),  &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
    eeprom_update_block ( pstSettings,                &(NVM_stSettings),               sizeof(stSettings));
#else
    eeprom_write_block ( &(uiVersion),               &(NVM_uiVersion),                sizeof(uiVersion)          );
    eeprom_write_block ( &(aucMagic[0]),             &(NVM_aucMagic[0]),              sizeof(aucMagic)           );
    eeprom_write_block ( &(atdKnownTempSensors[0]),  &(NVM_atdKnownTempSensors[0]),   sizeof(atdKnownTempSensors));
    eeprom_write_block ( pstSettings,                &(NVM_stSettings),               sizeof(stSettings));
#endif

    DEBUG_MEM( &(atdKnownTempSensors[0]),    sizeof(atdKnownTempSensors));
    DEBUG_MEM( pstSettings,                  sizeof(stSettings));

}
