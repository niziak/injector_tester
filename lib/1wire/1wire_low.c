/**
 * @file 1wire_low.c
 * @brief 1-Wire pulse, bit and byte routines
 * @author  Wojciech Niziński
 */
#include <1wire_config.h>
#include <1wire_low.h>

#define OW_MULTI_DEVICE 1   ///< support more than 1 sensor
#define OW_MAX_DEVIES 2
#define OW_CALIBRATION_TEST     0

#define OW_LOW_DEBUG                    0
#define OW_LOW_DEBUG_BITS_ON_WIRE       0
#define OW_LOW_DEBUG_BYTES_ON_WIRE      0
#define OW_LOW_DEBUG_SEARCH_ROM         0

#if (OW_LOW_DEBUG)
  #define OW_PRINTF(f,s...)     PRINTF(f, ##s)
  #define OW_PRINTF_P(f,s...)   PRINTF_P(f, ##s)
  #define OW_PRINTF_T_P(f,s...) PRINTF_P(f, ##s)
#else
  #define OW_PRINTF(x,s...)
  #define OW_PRINTF_P(x,s...)
  #define OW_PRINTF_T_P(x,s...)
#endif

#if 1
// ATmega328 16MHz
/********************/
/* standard timings */      // write slot minimum 60us with 1us recovery
/********************/
  #define DA    6    ///< write 1 pulse (6us) / also read pulse (master samples in 6+9=15us)
  #define DB    64   ///< write 1 - recovery to complete time slot 60us (6+54) + recovery time @ref DD between each slot

  #define DC    60   ///< write "0" pulse at least 60us  (keep for full time slot (at least 60us))

  #define DD    10   ///< recovery time between each slot

  #define DE    9    ///< master should read after 15us from start of read pulse, so after 9 us from previous pulse
  #define DF    55   ///< complete read slot to 60us + @ref DD 10us delay between slots (60-9-6=45us)

  #define DG    0     ///< reset - inital wait - ignored
  #define DH    480   ///< reset - pulse min 480us
  #define DI    70    ///< reset - wait for presence (after 550us)
  #define DJ    410   ///< reset - recovery (after 960us)
#endif


#if 0
// ARM SAM7X
/********************/
/* standard timings */      // write slot minimum 60us with 1us recovery
/********************/
  #define DA    6-5     /* read / write "1" pulse */            // must be released within 15us
  #define DB    64-20   /* recovery to complete time slot */     //

  #define DC    60-20   /* write "0" pulse */                   // // keep for full time slot (at least 60us)
  #define DD    10-5    /* recovery to complete time slot  */

  #define DE    9-9         /* read - sample */                 // redefined below to nothing
  #define DF    55+9-20     /* read - recovery */

  #define DG    0        /* reset - inital wait */
  #define DH    480-20   /* reset - pulse */                  //-20
  #define DI    70       /* reset - wait for presence */      //
  #define DJ    410-20   /* reset - recovery */               //-20
#endif

/*********************/
/* Overdrive timings */
/*********************/
  #define DAo    1       /* read / write "1" pulse */
  #define DBo    7       /* recovery to complete time slot */

  #define DCo    7       /* write "0" pulse */
  #define DDo    2       /* recovery to complete time slot  */

  #define DEo    1       /* read - sample */
  #define DFo    7       /* read - recovery */

  #define DGo    2       /* reset - inital wait */
  #define DHo    70      /* reset - pulse */
  #define DIo    8       /* reset - wait for presence */
  #define DJo    40      /* reset - recovery */

#if (OW_CALIBRATION_TEST)
  #include <1wire.h>
  #define UDELAY(x)    {OW_MICRO_DELAY( (unsigned int)x * (unsigned int)CalibrationDelayAdjust / 100);}
#else
  #define UDELAY(x)    {OW_MICRO_DELAY(x);}
#endif

//  NutIrqDisable(&sig_TC1);
//#define UDELAY(x)  {}

#define DELAY_A   UDELAY(DA)
#define DELAY_B   UDELAY(DB)
#define DELAY_C   UDELAY(DC)
#define DELAY_D   UDELAY(DD)
#define DELAY_E   UDELAY(DE)    // disabled on ARM
#define DELAY_F   UDELAY(DF)
#define DELAY_G   /*UDELAY(DG)*/    /* reset - inital wait - ignored */
#define DELAY_H   UDELAY(DH)
#define DELAY_I   UDELAY(DI)
#define DELAY_J   UDELAY(DJ)


#define DRIVE_BUS     {OW_0;OW_out;}
#define RELEASE_BUS   {OW_in;OW_1;}

#if (OW_CALIBRATION_TEST)
#define CALIBRATION_DELAY_ADJUST_MIN 20
#define CALIBRATION_DELAY_ADJUST_MAX 120
static uint8_t CalibrationDelayAdjust = CALIBRATION_DELAY_ADJUST_MIN;      ///< each delay will be multiplied by @ref CalibrationDelayAdjust/100

#endif


//INT              iTempFrac;   ///< current temperature in degrees (fractional part) Range 0-999
//INT              iTempInt;    ///< current temperature in degrees (integer part).

/**
 * Initializes the 1wire hardware. All transactions will begin with this hardware reset.
 * @return Returns nonzero if at least one 1wire slave is ready and connected to the bus
 */
uint8_t OWInit(void) OW_GCC_OPT2;
uint8_t OWInit(void)
{
    uint8_t r = 0;

    // check BUS for shorts:
    RELEASE_BUS
    if (OW_get==0)
      return 0;

    OW_CRITICAL_ENTER
    DELAY_G
    DRIVE_BUS
    DELAY_H
    RELEASE_BUS
    DELAY_I
      if (OW_get==0)
        r = 1;
    DELAY_J
    OW_CRITICAL_EXIT
    return r;
}

/*
 * Writes one bit to the 1wire bus.
 */
//inline void OWWriteBit (uint8_t bit) __attribute__((always_inline));
void OWWriteBit (uint8_t bit) OW_GCC_OPT2;
void OWWriteBit (uint8_t bit)
{
#if ((OW_LOW_DEBUG) && (OW_LOW_DEBUG_BITS_ON_WIRE))
  //OW_PRINTF ((" W%d ", bit));
#endif
  OW_CRITICAL_ENTER
  if (bit)
  {
    // write 1
    DRIVE_BUS       // low
    DELAY_A         // for 6us
    RELEASE_BUS     // release
    DELAY_B         // 54 recovery delay to reach 60us window
                    // + wait 10us between slots
  }
  else
  {
    //write 0
    DRIVE_BUS       // low
    DELAY_C         // wait 60us
    RELEASE_BUS     // release
    DELAY_D         // wait 10us between slots
  }
  OW_CRITICAL_EXIT
}

/**
 * @brief Reads and returns one bit from the bus.
 */
//inline uint8_t OWReadBit(void) __attribute__((always_inline));
uint8_t OWReadBit(void) OW_GCC_OPT2;
uint8_t OWReadBit(void)
{
  uint8_t r = 0;
  OW_CRITICAL_ENTER
  DRIVE_BUS     // low pulse
  DELAY_A       // for 6us
  RELEASE_BUS   // release - master should read after next 9 us (to fit into 15us window)
  DELAY_E       // wait 9us for slave pulse
    if (OW_get==1)  // read slave pulse
      r = 1;
  DELAY_F       // fill time to end of 60us slot, so 60-15=45us
                // plus 10us between slots
  OW_CRITICAL_EXIT
#if ((OW_LOW_DEBUG) && (OW_LOW_DEBUG_BITS_ON_WIRE))
  OW_PRINTF (" r%d ", r);
#endif
  return r;
}

/**
 * @bried Writes one byte (8 bits) to the 1wire bus.
 *
 * It simply calls DS1WWriteBit eight times for each bit.
 */
void OWWriteByte(uint8_t b) OW_GCC_OPT2;
void OWWriteByte(uint8_t b)
{
  uint8_t i;
#if ((OW_LOW_DEBUG) && (OW_LOW_DEBUG_BYTES_ON_WIRE))
  OW_PRINTF (" wb%02X ", b);
#endif
  //OW_CRITICAL_ENTER
  for (i=0; i<8; i++)
  {
    OWWriteBit(b & 0x01);
    b>>=1;
  }
  //OW_CRITICAL_EXIT
}

/**
 * @brief Reads and returns one byte (8 bits) from the 1wire bus.
 * It simply calls DS1WReadBit eight times for each bit.
 */
uint8_t OWReadByte(void) OW_GCC_OPT2;
uint8_t OWReadByte(void)
{
  uint8_t b = 0;
  uint8_t i;
  //OW_CRITICAL_ENTER
  for (i=0; i<8; i++)
  {
    b>>=1;
    if (OWReadBit())
    {
      b |= 0x80;
    }
  }
  //OW_CRITICAL_EXIT
#if ((OW_LOW_DEBUG) && (OW_LOW_DEBUG_BYTES_ON_WIRE))
  OW_PRINTF_P (PSTR(" rb%02X "), b);
#endif
  return b;
}


/**
 * @brief Performs a ROM search routine.
 *
 * It's used to find out the unique 64-bit serial
 * codes (ROM code) of each device. On each call the ROM code of the next found
 * device is written to *ROM, so be sure to provide a valid 8 byte sized buffer.
 * If the function returns -1, no device is found.
 * It returns 1 if a device is found and at least one can be found on the next
 * call.
 *
 * Finally it return 0 for the last found device.
 * To get the ROM codes of all devices, simply call DS1WSearchRom until it
 * returns 0 or -1. Do not clear provided buffer between calls!
 *
 * After search ROM, each device is sending its. First address bit is followed by complementary address bit.
 * Next, master sends slave bit. All slaves not equal to this bit are switched in search rom procedure.
 */
int OWSearchRom(uint8_t *ROM) OW_GCC_OPT2;
int OWSearchRom(uint8_t *ROM)
{
	OW_CRITICAL_ENTER
    static u_char lastDis = 0;
    u_char disMarker;
    u_char bits;
    u_char index;

    #if (OW_LOW_DEBUG_SEARCH_ROM)
      OW_PRINTF(" ");
    #endif
    /// - send reset signal
    if (!OWInit())
    {
        lastDis = 0;
        return -1;
    }

    disMarker = 0;
    ROM--;
    OWWriteByte(DS1W_SEARCH_ROM);

    for (index = 1; index <= (OW_ADDRESS_LEN*8); index++) /* 8 bytes rom code */
    {
        if ((index - 1) % 8 == 0)
        {
            #if (OW_LOW_DEBUG_SEARCH_ROM)
              OW_PRINTF(".");
            #endif
            ROM++;
        }
        bits = 0;
        if (OWReadBit())  // read address bit                   xxxx xxxB   B-address bit
            bits |= 0x01;

        if (OWReadBit())  // read complementary address bit     xxxx xxCB   C-complementary address bit
            bits |= 0x02;

        if (bits == 0x03) // two ones means, no more devices
        {
            lastDis = 0;
            #if (OW_LOW_DEBUG_SEARCH_ROM)
              OW_PRINTF("x");
            #endif
            return 0;
        }

        if (bits == 0)    // two zeros, more devices conflict on this bit
        {
            #if (OW_LOW_DEBUG_SEARCH_ROM)
              OW_PRINTF("c");
            #endif
            if (lastDis == index)
            {
                bits = 1;
            }
            else
            {
                if (index > lastDis)
                {
                    bits = 0;
                    disMarker = index;
                }
                else
                {
                    bits = (*(uint8_t*)ROM) & 0x01;
                    if (!bits)
                        disMarker = index;
                }
            }
        }
        else
        {
            bits &= 0x01;
            #if (OW_LOW_DEBUG_SEARCH_ROM)
              OW_PRINTF("%d", bits);
            #endif
        }

        (*(uint8_t*)ROM) >>= 1;

        if (bits)
        {
            (*(uint8_t*)ROM) |= 0x80;
        }
        OWWriteBit(bits);
    }
    lastDis = disMarker;
    OW_CRITICAL_EXIT
    #if (OW_LOW_DEBUG_SEARCH_ROM)
        OW_PRINTF(" ");
    #endif

    if (lastDis)
        return 1;
    else
        return 0;
}

#if (OW_CALIBRATION_TEST)
/*
 *
 */
void OWCalibrateDelays(void)
{
  // Reset wykrywa dla mnoznika od 50/100 do 185/100
  OW_PRINTF_T_P(PSTR("\nTest for delay * %d/100\n"), CalibrationDelayAdjust);
  OWDetectDevices();
  CalibrationDelayAdjust += 1;
  if (CalibrationDelayAdjust > CALIBRATION_DELAY_ADJUST_MAX)
    CalibrationDelayAdjust = CALIBRATION_DELAY_ADJUST_MIN;
}
#endif

/*
 *
 */
void OWWriteBuf (uint8_t *buf, size_t len) OW_GCC_OPT2;
void OWWriteBuf (uint8_t *buf, size_t len)
{
  for ( ; len; len--)
  {
    OWWriteByte(*(unsigned char*)buf);
    buf++;
  }
}

/*
 *
 */
void OWReadBuf (uint8_t *buf, size_t len) OW_GCC_OPT2;
void OWReadBuf (uint8_t *buf, size_t len)
{
  for (; len; len-- )
  {
    *(unsigned char*)buf = OWReadByte();
    buf++;
  }
}



