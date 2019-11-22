/*
 * MyTask.c
 *
 *  Created on: Nov 22, 2019
 *      Author: izzat
 */

#include "main.h"
#include "MyTask.h"
#include "MyLCD.h"
#include "MyUART.h"
#include "MySeOS.h"

unsigned char Button_Continue(void);
unsigned char Button_Abort(void);
unsigned char Button_C1000(void);
unsigned char Button_C500(void);

#define Continue HAL_GPIO_ReadPin(PbContinue_GPIO_Port, PbContinue_Pin)==GPIO_PIN_RESET
#define Abort HAL_GPIO_ReadPin(PbAbort_GPIO_Port, PbAbort_Pin)==GPIO_PIN_RESET
#define Pb1000  HAL_GPIO_ReadPin(PbC1000_GPIO_Port, PbC1000_Pin)==GPIO_PIN_RESET
#define pb500 HAL_GPIO_ReadPin(PbC500_GPIO_Port, PbC500_Pin)==GPIO_PIN_RESET

#define BuzzerOn HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET)
#define BuzzerOff HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET)

enum state {start, startdelay, c500, c1000, dropStuff, dropCoin} Condition;

volatile uint32_t timeout=0;
uint16_t nrp = 0;



_Bool RunEnable =1;

void MyTask_Init(void)
{
	timeout=0;
	Condition=start;
	LCD_Init();
}
void MyTask_Run(void)
{
	switch(Condition){
	case start:
	{
		break;
	}
	case startdelay:
	{
		break;
	}
	case c500:
	{
		break;
	}
	case c1000:
	{
		break;
	}
	case dropCoin:
	{
		break;
	}
	case dropStuff:
	{
		break;
	}
	}
}
void MyTask_Error_Handler(_Bool Enable, char *pData)
{

}
void MyTask_Display(_Bool Enable)
{
	if(Enable){
		LCD_SetCursor(0, 1);
		LCD_PrintNum(nrp);
	}
}

/*Sensor*/
unsigned char Button_Continue(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(Continue){
		if(++timeout>500000){
			BuzzerOn;
		}
		else{
			bouncing=(bouncing<<1);
		}
	}
	else{
		bouncing=(bouncing<<1)|1;
		timeout=0;
	}

	if(bouncing==0x03){
		UART_Print("Tranfer_OK");
		Flagdetect=1;
	}
	return Flagdetect;
}
unsigned char Button_Abort(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(Abort){
		bouncing=(bouncing<<1);
	}
	else{
		bouncing=(bouncing<<1)|1;
	}

	if(bouncing==0x03){
		UART_Print("Transfer_Cancel");
		BuzzerOff;
		Flagdetect=1;
	}
	return Flagdetect;
}
unsigned char Button_C1000(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(Pb1000){
		if(++timeout>500000){
			BuzzerOn;
		}
		else{
			bouncing=(bouncing<<1);
		}
	}
	else{
		bouncing=(bouncing<<1)|1;
		timeout=0;
	}

	if(bouncing==0x03){
		UART_Print("1000");
		Flagdetect=1;
	}
	return Flagdetect;
}
unsigned char Button_C500(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(pb500){
		if(++timeout>500000){
			BuzzerOn;
		}
		else{
			bouncing=(bouncing<<1);
		}
	}
	else{
		bouncing=(bouncing<<1)|1;
		timeout=0;
	}

	if(bouncing==0x03){
		UART_Print("500");
		Flagdetect=1;
	}
	return Flagdetect;
}



