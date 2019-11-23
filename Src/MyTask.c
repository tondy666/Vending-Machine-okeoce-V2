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


enum state {start, startdelay, finishdelay, kembali, buffer, index_start, c500, dropStuff, dropCoin} Condition;

volatile uint32_t timeout=0;
volatile uint8_t state_kembali=0;
unsigned int Coint_500=0;
unsigned int Coint_1000=0;
volatile uint64_t Jml_tunai = 0;
volatile uint16_t Coin=0;
volatile uint64_t Jml_kembali=0;
char dis_coint;


_Bool Kembali500 =0;
_Bool Kembali1000 =0;
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
		LCD_SetCursor(0, 0);LCD_Print("**Vending Mechine**");
		Condition=startdelay;
		break;
	}
	case startdelay:
	{
		BuzzerOn;
		if(++timeout>250000){
			LCD_SetCursor(0, 1);LCD_Print("Masukkan Coin:");
			timeout=0;
			BuzzerOff;
			MyTask_Display(RunEnable);
			Condition=index_start;
		}
		break;
	}
	case c500:
	{
		if(Button_C500()){
			Coin=5;
			Jml_tunai+=5;
			MyTask_Display(RunEnable);
			Condition=buffer;
		}
		break;
	}
	case buffer:
	{
		if(Button_C500()){
			Kembali500=!(Kembali500);
			Condition=kembali;
		}
		if(Button_C1000()){
			Kembali1000=!(Kembali1000);
			Condition=kembali;
		}
		if(Button_Continue()){
			Condition=dropStuff;
		}
		break;
	}
	case dropCoin:
	{
		break;
	}
	case dropStuff:
	{
		LCD_SetCursor(0, 3);LCD_Print("Minuman Keluar");
		Condition=finishdelay;
		break;
	}
	case index_start:
	{
		if(Button_C500()){
			Coin=5;
			Jml_tunai+=5;
			MyTask_Display(RunEnable);
			Condition=c500;
		}
		if(Button_C1000()){
			Coin=10;
			Jml_tunai+=10;
			MyTask_Display(RunEnable);
			Condition=buffer;
		}
		break;
	}
	case finishdelay:
	{
		if(++timeout>1000000){
			timeout=0;
			Coin=0;
			Jml_tunai=0;
			Jml_kembali=0;
			LCD_Clear();
			Condition=start;
		}
		break;
	}
	case kembali:
	{
		if(Kembali500){
			Coin=5;
			Jml_tunai=10;
			Jml_kembali+=5;
			Kembali500=!(Kembali500);
			MyTask_Display(RunEnable);
			Condition=buffer;
		}
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
		LCD_SetCursor(15, 1);LCD_PrintNum(Coin);LCD_Print("00 ");
		LCD_SetCursor(0, 2);LCD_Print("Total: ");LCD_PrintNum(Jml_tunai);LCD_Print("00 ");LCD_SetCursor(12, 2);LCD_PrintNum(Jml_kembali);LCD_Print("00 ");
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



