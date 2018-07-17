/*
 * LCD_HD44780.c
 *
 *	The MIT License.
 *  Created on: 11.07.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */
#include "main.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"

#include "delays.h"
#include "LCD_HD44780.h"

#define SET_LCD_E HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET)
#define RESET_LCD_E HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET)

#define SET_LCD_RS HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)
#define RESET_LCD_RS HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)

#ifdef USE_RW
#define SET_LCD_RW HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_SET)
#define RESET_LCD_RW HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET)
#endif

/*
 *
 * 	INSIDE FUNCTIONS
 *
 */

//
//	Set data port
//
static inline void LCD_SetDataPort(uint8_t data)
{

#ifdef LCD_4BIT
	if(data & (1<<0))
		HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_RESET);

	if(data & (1<<1))
		HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_RESET);

	if(data & (1<<2))
		HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_RESET);

	if(data & (1<<3))
		HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_RESET);
#else
	if(data & (1<<0))
		HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, GPIO_PIN_RESET);

	if(data & (1<<1))
		HAL_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, GPIO_PIN_RESET);

	if(data & (1<<2))
		HAL_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, GPIO_PIN_RESET);

	if(data & (1<<3))
		HAL_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, GPIO_PIN_RESET);

	if(data & (1<<4))
		HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_RESET);

	if(data & (1<<5))
		HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_RESET);

	if(data & (1<<6))
		HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_RESET);

	if(data & (1<<7))
		HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_RESET);
#endif
}

#ifdef USE_RW
static inline uint8_t LCD_GetDataPort()
{
	uint8_t result = 0;

#ifdef LCD_4BIT

	if(HAL_GPIO_ReadPin(LCD_DB4_GPIO_Port, LCD_DB4_Pin) == GPIO_PIN_SET) result |= (1<<0);
	if(HAL_GPIO_ReadPin(LCD_DB5_GPIO_Port, LCD_DB5_Pin) == GPIO_PIN_SET) result |= (1<<1);
	if(HAL_GPIO_ReadPin(LCD_DB6_GPIO_Port, LCD_DB6_Pin) == GPIO_PIN_SET) result |= (1<<2);
	if(HAL_GPIO_ReadPin(LCD_DB7_GPIO_Port, LCD_DB7_Pin) == GPIO_PIN_SET) result |= (1<<3);
#else
	if(HAL_GPIO_ReadPin(LCD_DB0_GPIO_Port, LCD_DB0_Pin) == GPIO_PIN_SET) result |= (1<<0);
	if(HAL_GPIO_ReadPin(LCD_DB1_GPIO_Port, LCD_DB1_Pin) == GPIO_PIN_SET) result |= (1<<1);
	if(HAL_GPIO_ReadPin(LCD_DB2_GPIO_Port, LCD_DB2_Pin) == GPIO_PIN_SET) result |= (1<<2);
	if(HAL_GPIO_ReadPin(LCD_DB3_GPIO_Port, LCD_DB3_Pin) == GPIO_PIN_SET) result |= (1<<3);
	if(HAL_GPIO_ReadPin(LCD_DB4_GPIO_Port, LCD_DB4_Pin) == GPIO_PIN_SET) result |= (1<<4);
	if(HAL_GPIO_ReadPin(LCD_DB5_GPIO_Port, LCD_DB5_Pin) == GPIO_PIN_SET) result |= (1<<5);
	if(HAL_GPIO_ReadPin(LCD_DB6_GPIO_Port, LCD_DB6_Pin) == GPIO_PIN_SET) result |= (1<<6);
	if(HAL_GPIO_ReadPin(LCD_DB7_GPIO_Port, LCD_DB7_Pin) == GPIO_PIN_SET) result |= (1<<7);
#endif

return result;
}
#endif

static void LCD_DataOut()
{
	  GPIO_InitTypeDef GPIO_InitStruct;
#ifdef LCD_4BIT
	  GPIO_InitStruct.Pin = LCD_DB4_Pin|LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
#else
	  GPIO_InitStruct.Pin = LCD_DB0_Pin|LCD_DB1_Pin|LCD_DB2_Pin|LCD_DB3_Pin
	                          |LCD_DB4_Pin|LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
#endif
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#ifdef USE_RW
static void LCD_DataIn()
{
	 GPIO_InitTypeDef GPIO_InitStruct;
#ifdef LCD_4BIT
	 GPIO_InitStruct.Pin = LCD_DB4_Pin|LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
#else
	 GPIO_InitStruct.Pin = LCD_DB0_Pin|LCD_DB1_Pin|LCD_DB2_Pin|LCD_DB3_Pin
	 	                          |LCD_DB4_Pin|LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
#endif
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//
//	Write byte to LCD
//
uint8_t LCD_ReadByte(void)
{
	uint8_t result = 0;
	LCD_DataIn();

	SET_LCD_RW;
#ifdef LCD_4BIT
	SET_LCD_E;
	result = (LCD_GetDataPort() << 4);
	RESET_LCD_E;

	SET_LCD_E;
	result |= LCD_GetDataPort();
	RESET_LCD_E;
#else
	SET_LCD_E;
	result = LCD_GetDataPort();
	RESET_LCD_E;
#endif
	return result;
}

//
//	Check Busy Flag
//
uint8_t LCD_CheckBusyFlag()
{
	RESET_LCD_RS;
	return LCD_ReadByte();
}
#endif

//
//	Write byte to LCD
//
void LCD_WriteByte(uint8_t data)
{
#ifdef USE_RW // There is no need to change GPIO direction if RW is not used
	LCD_DataOut();

	RESET_LCD_RW;
#endif

#ifdef LCD_4BIT
	SET_LCD_E;
	LCD_SetDataPort(data >> 4);
	RESET_LCD_E;
#endif
	SET_LCD_E;
	LCD_SetDataPort(data);
	RESET_LCD_E;


#ifdef USE_RW
	while((LCD_CheckBusyFlag() & (1<<7))); // Wait for data processing
#else
//		HAL_Delay(1);
		Delay_us(120); // Wait for data processing
#endif
}

//
//	Write command to LCD
//
void LCD_WriteCmd(uint8_t cmd)
{
	RESET_LCD_RS;
	LCD_WriteByte(cmd);
#ifndef USE_RW
	Delay_us(1000); //<<--- wait for command processing
#endif
}

//
//	Write data to LCD
//
void LCD_WriteData(uint8_t data)
{
	SET_LCD_RS;
	LCD_WriteByte(data);
}

/*
 *
 * 	USER FUNCTIONS
 *
 */

//
//	Write one character to LCD
//
void LCD_Char(char c)
{
	LCD_WriteData(((c >= 0x80) && (c <= 0x87)) ? (c & 0x07) : c);
}

//
//	Write string to LCD
//
void LCD_String(char* str)
{
	char c;
	while((c = *(str++)))
		LCD_Char(c);
}

//
// Print integer on LCD
//
void LCD_Int(int value)
{
	char buf[LCD_X+1];
	sprintf(buf, "%d", value);
	LCD_String(buf);
}

//
// Print hexadecimal on LCD
//
void LCD_Hex(int value, uint8_t upper_case)
{
	char buf[LCD_X+1];
	if(upper_case)
		sprintf(buf, "%X", value);
	else
		sprintf(buf, "%x", value);
	LCD_String(buf);
}

//
// Define custom char in LCD CGRAM in 'number' slot
//
void LCD_DefChar(uint8_t number, uint8_t *def_char)
{
	uint8_t i, c;
	LCD_WriteCmd(64+((number&0x7)*8));
	for(i = 0; i < 8; i++)
	{
		c = *(def_char++);
		LCD_WriteData(c);
	}
}

//
// Back to home position
//
void LCD_Home()
{
	LCD_WriteCmd(LCDC_CLS|LCDC_HOME);
}

//
// Control cursor visibility
//
void LCD_Cursor(uint8_t on_off)
{
	if(on_off == 0)
		LCD_WriteCmd(LCDC_ONOFF|LCDC_DISPLAYON);
	else
		LCD_WriteCmd(LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON);
}

//
// Control cursor blinking
//
void LCD_Blink(uint8_t on_off)
{
	if(on_off == 0)
		LCD_WriteCmd(LCDC_ONOFF|LCDC_DISPLAYON);
	else
		LCD_WriteCmd(LCDC_ONOFF|LCDC_DISPLAYON|LCDC_CURSORON|LCDC_BLINKON);
}

//
// Set cursor for x-column, y-row
//
void LCD_Locate(uint8_t x, uint8_t y)
{
	switch(y)
	{
		case 0:
			y = LCD_LINE1;
			break;
#if (LCD_Y>1)
		case 1:
			y = LCD_LINE2;
			break;
#endif
#if (LCD_Y>2)
		case 2:
			y = LCD_LINE3;
			break;
#endif
#if (LCD_Y>3)
		case 3:
			y = LCD_LINE4;
			break;
#endif
	}

	LCD_WriteCmd((0x80 + y + x));
}

//
//	Clear LCD
//
void LCD_Cls(void)
{
	LCD_WriteCmd(LCDC_CLS);
}

//
//	Initialization
//
void LCD_Init(void)
{
	RESET_LCD_RS;
	RESET_LCD_E;
#ifdef USE_RW
	RESET_LCD_RW;
#endif
	LCD_DataOut();

	HAL_Delay(15);

	LCD_SetDataPort(LCDC_FUNC|LCDC_FUNC8B);
	Delay_us(4100);
	LCD_SetDataPort(LCDC_FUNC|LCDC_FUNC8B);
	Delay_us(100);
#ifdef LCD_4BIT
	LCD_SetDataPort(LCDC_FUNC|LCDC_FUNC4B); //4-byte mode
	Delay_us(100);
	LCD_WriteCmd(LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7); // 4-bit, 2 lanes, 5x7 chars
#else
	LCD_SetDataPort(LCDC_FUNC|LCDC_FUNC8B);
	Delay_us(100);
	LCD_WriteCmd(LCDC_FUNC|LCDC_FUNC8B|LCDC_FUNC2L|LCDC_FUNC5x7); // 4-bit, 2 lanes, 5x7 chars
#endif

	LCD_WriteCmd(LCDC_ONOFF|LCDC_CURSOROFF); // Cursor off
	LCD_WriteCmd(LCDC_ONOFF|LCDC_DISPLAYON); // LCD on
	LCD_WriteCmd(LCDC_ENTRY|LCDC_ENTRYR); // Data entry right

	LCD_Cls(); // Clear display
}
