/*
 * MyUART.c
 *
 *  Created on: Nov 22, 2019
 *      Author: izzat
 */

#include "main.h"
#include <string.h>
#include <stdio.h>

UART_HandleTypeDef huart2;

void UART_Print(char *pData)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)pData, strlen(pData), 10);
}
void UART_Println( char *pData)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)pData, strlen(pData), 10);
	char newLine[2] = "\n\r";
	HAL_UART_Transmit(&huart2, (uint8_t *)newLine, 2, 10);
}
void UART_PrintNum(uint16_t NumVal)
{
	char buff[100];
	sprintf(buff, "%d", NumVal);
	HAL_UART_Transmit(&huart2, (uint8_t *)buff, strlen(buff), 10);

}
