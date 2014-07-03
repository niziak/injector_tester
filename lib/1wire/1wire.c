/**
 * @file 1wire_low.c
 * @brief 1-Wire pulse, bit and byte routines
 * @author  Wojciech Niziński
 */

#include <1wire.h>
#include <1wire_low.h>
#include <stdlib.h>
#include <string.h>

#define OW_MAX_DEVICES   (NUM_OF_TEMP_SENSORS)
//#define OW_CALIBRATION_TEST

#define OW_DEBUG   1

#if (OW_DEBUG)
  #define OW_PRINTF(f,s...)     PRINTF(f, ##s)
  #define OW_PRINTF_P(f,s...)   PRINTF_P(f, ##s)
  #define OW_PRINTF_T_P(f,s...) PRINTF_P(f, ##s)
#else
  #define OW_PRINTF(x,s...)
  #define OW_PRINTF_P(x,s...)
  #define OW_PRINTF_T_P(x,s...)
#endif

#if (OW_CALIBRATION_TEST)
  #define UDELAY(x)    {NutMicroDelay(x*test/100);}
#else
  #define UDELAY(x)    {NutMicroDelay(x);}
#endif

//  NutIrqDisable(&sig_TC1);
//#define UDELAY(x)  {}


#if (OW_CALIBRATION_TEST)
static uint8_t test = 1;
#endif


#if (OW_CALIBRATION_TEST)
/*
 *
 */
void OWCalibrateDelays(void)
{
  // Reset wykrywa dla mnoznika od 50/100 do 185/100
  OW_PRINTF_T("\nTest for %d\n", test);
  OWDetectDevices(stdout);
  test+=1;
  if (test>20)
    test = 1;
}
#endif


/*
 *  TM_Crc - calculate CRC (Cyclic Redundancy Check) for
 *           Dallas Semiconductor Touch Memory.
 */
static u_char TM_Crc(u_char crc, u_char inp) OW_GCC_OPT2;
static u_char TM_Crc(u_char crc, u_char inp)
{
    u_char inp_z = inp,
           i = 8,
           carry;
    do
    {
        inp ^= crc;
        crc >>= 1;

        if (inp & 1)
        {
            crc ^= (0x18 >> 1);
            crc |= 0x80;
        }

        carry = inp_z & 1;
        inp_z >>= 1;

        if (carry)
        {
            inp_z |= 0x80;
        }
        inp = inp_z;

    } while (--i);

    return crc;
}



/**
 * Detects and prints 1wire devices
 *
 * @remark: Structure atdOWCurrentDevices[] is updated with currently detected sensors.
 *          Also list of stored and detected sensors are double linked using pointers by array index:
 *            position of sensor in @ref stSettings.atdTempSensor[]
 *            position of sensor in @ref atdOWCurrentDevices[]
 */
void OWDetectDevices(void)
{
  uint8_t aucROM[OW_ADDRESS_LEN];
  uint8_t i,num,a;
  int result = 1;
  BOOL bStored;
  OW_PRINTF_P(PSTR("\t1W bus answer: %d\n"), OWInit());

  // clear all detected sensors
  memset (&OW_NEW_DEVICE_ARRAY(0), 0, OW_NEW_DEVICE_ARRAY_SIZE);

  // set all known sensors to offline
  for (a=0; a < NUM_OF_TEMP_SENSORS; a++)
  {
	  OW_KNOWN_TEMP_SENSOR_STATUS(a) = 0;
  }
  memset (aucROM, 0, sizeof(aucROM)); // do memset only once, OWSearchRom is using previous address to find next one
  num = 0;

  while ((num < NUM_OF_TEMP_SENSORS) && (result>0))
  {
    result = OWSearchRom(&(aucROM[0]));

    OW_PRINTF_P (PSTR("\t%d\t"), num);
    for (i=0; i<sizeof(aucROM); ++i)
    {
      OW_PRINTF_P (PSTR("%02X "), aucROM[i]);
    }

    // find device and set status
    bStored = FALSE;
    for (a=0; a < NUM_OF_TEMP_SENSORS; a++)
    {
      if (memcmp (&(aucROM[0]), &OW_KNOWN_TEMP_SENSOR_ROM(a,0), OW_ADDRESS_LEN) == 0)
      {
        bStored = TRUE;
        OW_KNOWN_TEMP_SENSOR_STATUS(a) = 1;
#if (OW_WITH_SENSOR_NAMES)
        OW_PRINTF_P(PSTR(" stored as '%s'"), OW_KNOWN_TEMP_SENSOR_NAME(a));
#else
        OW_PRINTF_P(PSTR(" stored"));
#endif
      }
    }
    if (!bStored)
    {
        // store not known ID to atdOWNewDevices[] array
        memmove (&OW_NEW_DEVICE_ARRAY_ELEM(num,0), &(aucROM[0]), OW_NEW_DEVICE_ARRAY_ELEM_SIZE(num));
        OW_PRINTF_P(PSTR(" new"));
    }
    OW_PRINTF_P (PSTR("\n"));
    num++;
  }
}

/**
 *  TM_Convert_temperature()
 *
 *  This function converts value from raw into an integer data format.
 *  Conversion is chosen based on an 8-bit family code.
 *
 *  Assume: 0x10 - DS1820,
 *          0x10 - DS18S20 - software compatible with the DS1820.
 *          0x22 - DS1822
 *          0x28 - DS18B20 - software compatible with the DS1822.

 *
 *
 *  raw temp=71   0x0047   47 00 4B 46 7F FF 09 10 AE   temp="4.0"
    raw temp=40   0x0028   28 00 4B 46 7F FF 08 10 71   temp="2.0"
    raw temp=21   0x0015   15 00 4B 46 7F FF 0B 10 A4   temp="1.0"
    raw temp=8    0x0008   08 00 4B 46 7F FF 08 10 D9   temp="0.0"
    raw temp=4092 0x0FFC   FC FF 4B 46 7F FF 04 10 A0   temp="-32641.0"
    raw temp=4087 0x0FF7   F7 FF 4B 46 7F FF 09 10 C6   temp="-32641.0"
                     |||
                     ||+-- fractional part
                     ||
 *                   |+--- temp (0x80 - sign)
 *                   +---- temp
 *
 * TODO: make CUNIT tests:
 *
   TEMPERATURE (°C)      DIGITAL OUTPUT(BINARY)      DIGITAL OUTPUT(HEX)
                          sign| integer |frac
    +127.9                0000 0111 1111 1111           07FFh <-- wrong value but sometimes occurs
    +125                  0000 0111 1101 0000           07D0h
    +85*                  0000 0101 0101 0000           0550h
    +25.0625              0000 0001 1001 0001           0191h
    +10.125               0000 0000 1010 0010           00A2h
    +0.5                  0000 0000 0000 1000           0008h
     0                    0000 0000 0000 0000           0000h
    -0.5                  1111 1111 1111 1000           FFF8h
    -10.125               1111 1111 0101 1110           FF5Eh
    -25.0625              1111 1110 0110 1111           FE6Fh
    -55                   1111 1100 1001 0000           FC90h
 */

void TM_Convert_temperature(UCHAR idx, UINT *temp, UINT *frac)
{
#if (OW_DEBUG)
  OW_PRINTF_P(PSTR("TM_Convert_temperature(%d, %d, %d)\n"), idx, *temp, *frac);
#endif

  switch(OW_KNOWN_TEMP_SENSOR_ROM(idx,0))
  {
#if FALSE
    case 0x10:
      if(*temp & 0x01) *frac = 5000; /* 4 decimal char. precision */
      else             *frac = 0;
      *temp >>= 1;
      break;
#endif
    case 0x22:
    case 0x28:
      /* 16 bit U2 value, frac part on 4 LSb bits[3:0], sign occupies MSb [15:11] */
      *frac = ((*temp & (1 << 3)) ? 10000/(1 << 1) : 0) +
              ((*temp & (1 << 2)) ? 10000/(1 << 2) : 0) +
              ((*temp & (1 << 1)) ? 10000/(1 << 3) : 0) +
              ((*temp & (1 << 0)) ? 10000/(1 << 4) : 0);
      *temp >>= 4;
       break;
    default:
#if (OW_DEBUG)
        OW_PRINTF_P(PSTR("\n\rUnknown family code - %02X.\n\r"), OW_KNOWN_TEMP_SENSOR_ROM(idx,0));
#endif
       *temp = 85;
       *frac = 0;
        break;
  }

  /** restore sign bits after shift */
  if(*temp & 0x0F00)    /* check the sign bit */
  {
    //*temp &= 0x007F;    /* clear sign bits */
    *temp |= 0xF000;    /* set sign bit */
  }
}

/**
 * Configure all devices on list stSettings.atdTempSensor[]
 *  sets alarm temperatures
 *  sets resolution
 */
void vOWConfigDevices(void) OW_GCC_OPT2;
void vOWConfigDevices(void)
{
#if (OW_DEBUG)
  OW_PRINTF_P(PSTR("vOWConfigDevices()\n"));
#endif
  UCHAR ucSensIdx;
  //unsigned char aucBuf[9] = { 0, 0, 0x4B, 0x46, 0x7F, 0, 0, 0, 0xFF };
  unsigned char aucBuf[3] = { 0x4B, 0x46, DS1W_RES_12 };

  for (ucSensIdx=0; ucSensIdx<NUM_OF_TEMP_SENSORS; ucSensIdx++)
   {
     if (OW_KNOWN_TEMP_SENSOR_ROM(ucSensIdx, 0) == 0x00)
     {
       // skip empty entries
       continue;
     }

     OWInit();
     OWWriteByte(DS1W_MATCH_ROM); // skip rom, starts conversion on ALL 1-wire devices. TODO match rom and set rom address
     OWWriteBuf (&OW_KNOWN_TEMP_SENSOR_ROM(ucSensIdx,0), OW_ADDRESS_LEN);
     OWWriteByte(DS1W_WRITE_SCRATCHPAD);
     OWWriteBuf(&aucBuf[0], sizeof(aucBuf));
     OWWriteByte(DS1W_COPY_SCRATCHPAD);
   }
}


/**
 * Start conversion on all 1wire thermometers.
 * To prevent self-heating of thermometer it should be started only every 10 seconds
 */
void OW_vStartConversion(void) OW_GCC_OPT2;
void OW_vStartConversion(void)
{
#if (OW_DEBUG)
  OW_PRINTF_P(PSTR("vOWStartConversion()\n"));
#endif
  OWInit();
  OWWriteByte(DS1W_SKIP_ROM); // skip rom, starts conversion on ALL 1-wire devices. TODO match rom and set rom address
  OWWriteByte(DS1W_CONVERT_T);
}

/*
 *
 */
UINT uiOWGetTemp(UCHAR *pucROM) OW_GCC_OPT2;
UINT uiOWGetTemp(UCHAR *pucROM)
{
  unsigned char aucBuf[9];
  u_int temper;
  u_char i;
  u_char crc;
  #define crc_read  ((u_char)aucBuf[8]) /* last byte is CRC */

  OWInit();
  OWWriteByte(DS1W_MATCH_ROM);
  #if OW_DEBUG
    OW_PRINTF_P(PSTR("\n\nGet temp for\t"));
    for (i=0; i<OW_ADDRESS_LEN; i++)
      OW_PRINTF_P(PSTR("%02X "), pucROM[i]);
    OW_PRINTF_P(PSTR("\n"));
  #endif
  OWWriteBuf (pucROM, OW_ADDRESS_LEN);
  OWWriteByte(DS1W_READ_SCRATCHPAD);
  OWReadBuf (&aucBuf[0], sizeof(aucBuf));

  crc=0;
  for (i=0; i<sizeof(aucBuf); i++)
  {
    crc = TM_Crc(crc, aucBuf[i]);
  }
  temper = ((aucBuf[1] /*& 0x0F*/) << 8) | aucBuf[0];
  if (crc)
  {
    temper = OW_TEMP_ERROR; /* return error (conversion can be pending) */
    OW_PRINTF_T_P(PSTR("\n 1-wire CRC err: %04X (%02X != %02X)\n"), temper, crc, crc_read);
  }
  #if OW_DEBUG
    OW_PRINTF_P(PSTR("1W data:\t"));
    for (i=0; i<sizeof(aucBuf); i++)
    {
      OW_PRINTF_P (PSTR("%02X "), aucBuf[i]);
    }
    OW_PRINTF_P (PSTR("\nraw temp=%d 0x%04X\n"), temper, temper);
  #endif
  //return 0xFFFC;
  return temper;
}

/**
 * Process 1wire results
 */
void OW_vWorker()
{
  UCHAR ucSensIdx;

  //OWDetectDevices(); // detect and print devices to console TODO maybe mutex to access from http ?
  vOWConfigDevices(); // set resultion etc...

//  OW_vStartConversion();
//  OW_SLEEP_MS (900); // wait for conversion result at least 750ms

  for (ucSensIdx=0; ucSensIdx<NUM_OF_TEMP_SENSORS; ucSensIdx++)
  {
    // mark temp as invalid, before read in case sensor is not known etc
    OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx) = OW_TEMP_ERROR;
    // read temperature from stored only sensors
    if (OW_KNOWN_TEMP_SENSOR_ROM(ucSensIdx,0) == 0x00)
    {
      // skip empty entries
      continue;
    }
    OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx) = uiOWGetTemp(&(OW_KNOWN_TEMP_SENSOR_ROM(ucSensIdx, 0)));
    if (OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx) == OW_TEMP_ERROR)
    {
      //stSettings.atdTempSensor[ucSensIdx].iTempInt = TEMP_ERROR;
      continue;
    }
    TM_Convert_temperature(ucSensIdx, (UINT*)&OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx), (UINT*)&OW_KNOWN_TEMP_SENSOR_TEMP_FRAC(ucSensIdx));
    OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx) += (INT)OW_KNOWN_TEMP_SENSOR_TEMP_ADJ(ucSensIdx);
    #ifdef OW_MAX_TEMP
        if ( OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx) > OW_MAX_TEMP)
            OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx) = OW_MAX_TEMP;
    #endif
    //iTempInt*= -1; // TEMP add minus
    #if OW_DEBUG
      OW_PRINTF_P(PSTR(" #%d human temp=\"%d.%01d\" \n"), ucSensIdx, OW_KNOWN_TEMP_SENSOR_TEMP_INT(ucSensIdx), OW_KNOWN_TEMP_SENSOR_TEMP_FRAC(ucSensIdx)/1000); /* precision 0.1 degree centigrade */
    #endif
  }
#if 0 /* conversion tests */
  (iTempInt)=0x07D0;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0x0550;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0x0191;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0x00A2;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0x0008;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0x0000;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0xFFF8;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0xFF5E;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0xFE6F;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
  (iTempInt)=0xFC90;  TM_Convert_temperature(0, (UINT*)&(iTempInt), (UINT*)&iTempFrac); PRINTF(" temp=\"%d.%01d\" \n",iTempInt, iTempFrac/1000); /* precision 0.1 degree centigrade */
#endif
  /*
  NutSleep(200);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  OWWriteByte(0xFF);
  NutSleep(200);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  OWWriteByte(0x0);
  NutSleep(200);
  */

}


