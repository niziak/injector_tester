/*
 * lcd_buff.c
 *
 *  Created on: Aug 9, 2013
 *      Author: nizinski_w
 */

#include <config.h>

#include <hal_lcd_low.h>
#include <types.h>
#include <lcd_buff_config.h>
#include <string.h>
#include <lcd_buff.h>

typedef struct
{
    UCHAR aucBuf[LCD_COLS*LCD_ROWS];
    UCHAR ucCursorX;
    UCHAR ucCursorY;
    UCHAR ucCursorType; // 0 - hidden, 1 - visible
#if LCD_ROWS>2
    #error "Maximum LCD_ROWS=2 implemented"
    UCHAR aucRow3[LCD_COLS];
#endif
#if LCD_ROWS>3
    UCHAR aucROw4[LCD_COLS];
#endif
#if LCD_ROWS>4
    #error "Maximum LCD_ROWS=4 allowed"
#endif
} LCD_VBUF_DEF;

static LCD_VBUF_DEF     tdLCDBuf;
#define ptdLCDBuf       (&(tdLCDBuf))

#define LCD_MEM_POS(x,y)    (x+(y*LCD_COLS))

/**
 * Initialize LCD hardware and also internal screen buffer
 */
void LCD_vInit(void)
{
    LCD_DEBUG_P(PSTR("LCD_vInit\n"))
    LCD_LO_vInit();
    LCD_vClrScr();
}

/**
 * Clear internal buffer
 */
void LCD_vClrScr(void)
{
    LCD_DEBUG_P(PSTR("LCD_vClrScr\n"))
    memset (ptdLCDBuf->aucBuf, 0, sizeof(ptdLCDBuf->aucBuf));
    LCD_vHome();
}

/**
 *
 * @param ucChar
 */
void LCD_vPutc(unsigned char ucChar)
{
    LCD_DEBUG_P(PSTR("LCD_vPutc\n"))
    ptdLCDBuf->aucBuf[LCD_MEM_POS(ptdLCDBuf->ucCursorX, ptdLCDBuf->ucCursorY)] = ucChar;
    ptdLCDBuf->ucCursorX++;
    if (ptdLCDBuf->ucCursorX > LCD_COLS)
    {
        LCD_DrawDebug();
        RESET_P(PSTR("ucCursorX OV!"));
    }

}

/**
 *
 * @param pucString
 */
void LCD_vPuts(const char *pucString)
{
    UCHAR ucC;
    LCD_DEBUG_P(PSTR("LCD_vPuts\n"))
    while ((ucC = *pucString++))
    {
        LCD_vPutc(ucC);
    }
}


/**
 *
 * @param pucString
 */
void LCD_vPuts_P(const char *pucPGMString)
{
    UCHAR ucC;
    LCD_DEBUG_P(PSTR("LCD_vPuts_P\n"))
    while ((ucC = pgm_read_byte(pucPGMString++)))
    {
        LCD_vPutc(ucC);
    }
}

void LCD_vHome(void)
{
    LCD_DEBUG_P(PSTR("LCD_vHome\n"))
    ptdLCDBuf->ucCursorX = 0;
    ptdLCDBuf->ucCursorY = 0;
}

void LCD_vGotoXY(unsigned char x, unsigned char y)
{
    LCD_DEBUG_P(PSTR("LCD_vGotoXY\n"))
    ptdLCDBuf->ucCursorX = x;
    ptdLCDBuf->ucCursorY = y;
}


/**
 * Draw whole display buffer on real LCD device
 * Can be called from interrupt
 */
void LCD_Draw(void)
{
    UCHAR ucRow, ucCol;
    UCHAR ucChar;
    LCD_DEBUG_P(PSTR("LCD_Draw\n"))

    // replace zero to spaces
    for (ucRow=0; ucRow < sizeof (ptdLCDBuf->aucBuf); ucRow++)
    {
        switch (ptdLCDBuf->aucBuf[ucRow])
        {
            case 0:
                ptdLCDBuf->aucBuf[ucRow] = ' ';
                break;
        }
    }

    // REAL LCD (to prevent flickers, needs to be fast and simple)
    //LCD_LO_vClrScr();
    LCD_LO_vHome();
    for (ucRow=0; ucRow < LCD_ROWS; ucRow++)
    {
        for (ucCol=0; ucCol < LCD_COLS; ucCol++)
        {
            ucChar = ptdLCDBuf->aucBuf[LCD_MEM_POS(ucCol,ucRow)];
            LCD_LO_vPutc(ucChar);
        }
        LCD_LO_vGotoXY (0,ucRow+1);
    }

    // show real cursor if needed
    if (ptdLCDBuf->ucCursorType == 1)
    {
        LCD_LO_vGotoXY (ptdLCDBuf->ucCursorX, ptdLCDBuf->ucCursorY);
    }

}

/**
 * Draw content of LCD buffer on serial console in form like:
 *
 * +----------------+
 * |Nowy czas:      |
 * |[ 5:04:30]      |
 * +----------------+
 *
 */
extern void LCD_DrawDebug(void)
{
    UCHAR ucRow, ucCol;
    UCHAR ucChar;
    BOOL bCursorOn;

#if (LCD_BUFF_DUMP_BUFFER)
    LCD_DEBUG_P(PSTR("LCD_Draw\n"))

    // replace zero to spaces
    for (ucRow=0; ucRow < sizeof (ptdLCDBuf->aucBuf); ucRow++)
    {
        switch (ptdLCDBuf->aucBuf[ucRow])
        {
            case 0:
                ptdLCDBuf->aucBuf[ucRow] = ' ';
                break;
        }
    }
    // DEBUG OUTPUT

    LOG_Log_P(PSTR("\n+----------------+\n"));
    for (ucRow=0; ucRow < LCD_ROWS; ucRow++)
    {
        LOG_Log_P(PSTR("|"));
        for (ucCol=0; ucCol < LCD_COLS; ucCol++)
        {
            // get character to print
            ucChar = ptdLCDBuf->aucBuf[LCD_MEM_POS(ucCol,ucRow)];

            // draw cursor - ANSI: underline attribute
            bCursorOn = FALSE;
            if (    (1 == ptdLCDBuf->ucCursorType)
                 && ((ptdLCDBuf->ucCursorX == ucCol) && (ptdLCDBuf->ucCursorY == ucRow)) )
            {
                LOG_Log_P(PSTR("\e[4m"));
                bCursorOn = TRUE;
            }

                // show only printable characters
                if ((ucChar < 0x20) || (0x7E < ucChar))
                {
                   ucChar = '.';
                }
                LOG_Log_P(PSTR("%c"), ucChar);

            if (bCursorOn)
            {
                LOG_Log_P(PSTR("\e[0m")); // ANSI: reset all attributes
            }
        }
        LOG_Log_P(PSTR("|"));
        LOG_NL
    }
    LOG_Log_P(PSTR("+----------------+\n"));
#endif //(LCD_BUFF_DUMP_BUFFER)
}


void LCD_vCursorShow(void)
{
//    LCD_LO_vGotoXY (ptdLCDBuf->ucCursorX, ptdLCDBuf->ucCursorY);
    ptdLCDBuf->ucCursorType = 1;
    LCD_LO_vCursorShow();
}

void LCD_vCursorHide(void)
{
    ptdLCDBuf->ucCursorType = 0;
    LCD_LO_vCursorHide();
}
