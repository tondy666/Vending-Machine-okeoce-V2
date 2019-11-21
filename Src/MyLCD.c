/*
 * MyLCD.c
 *
 *  Created on: Nov 21, 2019
 *      Author: izzat
 */

#include "main.h"
#include "MyLCD.h"
#include <stdio.h>


void LCD_Write4Bit(uint8_t data)
{
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, ((data>>0)&1)? GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, ((data>>1)&1)? GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, ((data>>2)&1)? GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, ((data>>3)&1)? GPIO_PIN_SET:GPIO_PIN_RESET);
}
void LCD_Send(_Bool mode, uint8_t data)
{
	LCD_RW(0);
	if(mode){
		LCD_RS(1);
	}else{
		LCD_RS(0);
	}

	LCD_Write4Bit(data>>4);
	LCD_EN();
	LCD_Write4Bit(data);
	LCD_EN();
}
void LCD_Init(void)
{
	LCD_RW(0);

	LCD_Command(0x33);
	LCD_Command(0x32);
	LCD_Command(0x28);
	LCD_Command(0x0C);
	LCD_Command(0x06);
}
void LCD_Print(char *pData)
{
	while(*pData){
		LCD_Data(*pData++);
	}
}
void LCD_PrintNum(uint16_t numVal)
{
	char bufprint[20];
	sprintf(bufprint, "%d", numVal);
	LCD_Print(bufprint);
}
void LCD_SetCursor(uint8_t x, uint8_t y)
{
	if(y==0){
		LCD_Command(0x80+x);
	}
	if(y==1){
		LCD_Command(0xC0+x);
	}
	if(y==2){
		LCD_Command(0x94+x);
	}
	if(y==3){
		LCD_Command(0xD4+x);
	}
}
void LCD_Clear(void)
{
	LCD_Command(0x01);
}
