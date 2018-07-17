/*
 * LCD_HD44780.h
 *
 *	The MIT License.
 *  Created on: 11.07.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */

#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_

/*
 *
 * 	SETTINGS
 *
 */
#define LCD_Y 2
#define LCD_X 16

// RW pin using
#define USE_RW

// Interface width
#define LCD_4BIT

#ifndef LCD_4BIT
#define LCD_8BIT
#endif

/*
 *
 * 	DEFINES
 *
 */
#if ( (LCD_Y == 4) && (LCD_X == 16) )
#define LCD_LINE1 0x00		// adres 1 znaku 1 wiersza
#define LCD_LINE2 0x28		// adres 1 znaku 2 wiersza
#define LCD_LINE3 0x14  	// adres 1 znaku 3 wiersza
#define LCD_LINE4 0x54  	// adres 1 znaku 4 wiersza
#else
#define LCD_LINE1 0x00		// adres 1 znaku 1 wiersza
#define LCD_LINE2 0x40		// adres 1 znaku 2 wiersza
#define LCD_LINE3 0x10  	// adres 1 znaku 3 wiersza
#define LCD_LINE4 0x50  	// adres 1 znaku 4 wiersza
#endif

#define LCDC_CLS					0x01
#define LCDC_HOME					0x02
#define LCDC_ENTRY					0x04
#define LCDC_ENTRYR					0x02
#define LCDC_ENTRYL					0x00
#define LCDC_MOVE					0x01
#define LCDC_ONOFF					0x08
#define LCDC_DISPLAYON				0x04
#define LCDC_CURSORON				0x02
#define LCDC_CURSOROFF				0x00
#define LCDC_BLINKON				0x01
#define LCDC_SHIFT					0x10
#define LCDC_SHIFTDISP				0x08
#define LCDC_SHIFTR					0x04
#define LCDC_SHIFTL					0x00
#define LCDC_FUNC					0x20
#define LCDC_FUNC8B					0x10
#define LCDC_FUNC4B					0x00
#define LCDC_FUNC2L					0x08
#define LCDC_FUNC1L					0x00
#define LCDC_FUNC5x10				0x04
#define LCDC_FUNC5x7				0x00
#define LCDC_SET_CGRAM				0x40
#define LCDC_SET_DDRAM				0x80

/*
 *
 * 	FUNCTIONS
 *
 */

void LCD_Init(void);
void LCD_Cls(void);
void LCD_Locate(uint8_t x, uint8_t y);
void LCD_Home();
void LCD_Cursor(uint8_t on_off);
void LCD_Blink(uint8_t on_off);

void LCD_Char(char c);
void LCD_String(char* str);

void LCD_Int(int value);
void LCD_Hex(int value, uint8_t upper_case);

void LCD_DefChar(uint8_t number, uint8_t *def_char);

#endif /* LCD_HD44780_H_ */
