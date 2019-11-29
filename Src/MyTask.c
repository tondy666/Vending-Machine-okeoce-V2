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
void Led_Init(void);
void Led_Process(void);
void Led_Cancel(void);
void Led_kembali500(void);
void Led_Kembali1000(void);
void Led_Kembali(void);
void Led_500(void);

#define Continue HAL_GPIO_ReadPin(PbContinue_GPIO_Port, PbContinue_Pin)==GPIO_PIN_RESET
#define Abort HAL_GPIO_ReadPin(PbAbort_GPIO_Port, PbAbort_Pin)==GPIO_PIN_RESET
#define Pb1000  HAL_GPIO_ReadPin(PbC1000_GPIO_Port, PbC1000_Pin)==GPIO_PIN_RESET
#define pb500 HAL_GPIO_ReadPin(PbC500_GPIO_Port, PbC500_Pin)==GPIO_PIN_RESET

#define BuzzerOn HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET)
#define BuzzerOff HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET)

enum state {start, startdelay, finishdelay, koinkurang, kembali, buffer, index_start, c500, dropStuff, dropCoin, batal} Condition;

volatile uint32_t timeout=0;
volatile uint32_t i=0;
volatile uint8_t state_kembali=0;
unsigned int Coint_500=0;
unsigned int Coint_1000=0;
volatile uint64_t Jml_tunai = 0;
volatile uint16_t Coin=0;
volatile uint16_t Coin_temp=0;
volatile uint64_t Jml_kembali=0;
char dis_coint;

_Bool Kembali500 =0;
_Bool Kembali1000 =0;
_Bool Abort500=0;
_Bool Abort1000=0;
_Bool AbortTotal=0;
_Bool RunEnable =1;

void MyTask_Init(void)
{
	timeout=0;
	Condition=start;
	Led_Init();
	LCD_Init();
}
void MyTask_Run(void)
{
	switch(Condition){
	case start:
	{
		Led_Init();
		LCD_SetCursor(0, 0);LCD_Print("**Vending  Mechine**");
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
			Coin_temp+=5;
			Jml_tunai+=5;
			MyTask_Display(RunEnable);
			Condition=buffer;
		}
		if(Button_C1000()){
			Coin=10;
			Coin_temp+=10;
			Kembali500=!(Kembali500);
			Led_kembali500();
			Condition=kembali;
		}
		if(Button_Abort()){
			LCD_Clear();
			LCD_SetCursor(0, 0);LCD_Print("Transaksi gagal");
			Led_kembali500();
			Condition=batal;
		}
		if(Button_Continue()){
			BuzzerOn;
			LCD_Clear();
			LCD_SetCursor(0, 1);LCD_Print("**Can not Process**");
			LCD_SetCursor(0, 2);LCD_Print("** Koin Kurang!! **");
			Condition=koinkurang;
		}
		break;
	}
	case koinkurang:
	{
		if(++timeout>1000000){
			timeout=0;
			LCD_Clear();
			LCD_SetCursor(0, 0);LCD_Print("**Vending  Mechine**");
			LCD_SetCursor(0, 1);LCD_Print("Masukkan Coin:");
			MyTask_Display(RunEnable);
			BuzzerOff;
			Condition=c500;
		}
		break;
	}
	case buffer:
	{
		Led_Init();
		if(Button_C500()){
			Coin=5;
			Coin_temp+=5;
			Kembali500=!(Kembali500);
			Led_500();
			Condition=kembali;
		}
		if(Button_C1000()){
			Coin=10;
			Coin_temp+=10;
			Kembali1000=!(Kembali1000);
			Led_Kembali1000();
			Condition=kembali;
		}
		if(Button_Continue()){
			Condition=dropStuff;
		}
		if(Button_Abort()){
			LCD_Clear();
			LCD_SetCursor(0, 0);LCD_Print("Transaksi gagal");
			if(Coin_temp>12)
			{
				Led_Cancel();
			}else{
				Led_Kembali();
			}
			Condition=batal;
		}
		break;
	}
	case dropCoin:
	{
		break;
	}
	case dropStuff:
	{
		Led_Process();
		LCD_SetCursor(0, 3);LCD_Print("Minuman Keluar");
		Condition=finishdelay;
		break;
	}
	case index_start:
	{
		if(Button_C500()){
			Coin=5;
			Coin_temp+=5;
			Jml_tunai+=5;
			MyTask_Display(RunEnable);
			Condition=c500;
		}
		if(Button_C1000()){
			Coin=10;
			Coin_temp+=10;
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
			Coin_temp=0;
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
			if(++timeout>500000){
				Jml_tunai=10;
				Jml_kembali+=5;
				Kembali500=!(Kembali500);
				MyTask_Display(RunEnable);
				timeout=0;
				Condition=buffer;
			}
		}
		if(Kembali1000){
			if(++timeout>500000){
				Jml_tunai=10;
				Jml_kembali+=10;
				Kembali1000=!(Kembali1000);
				MyTask_Display(RunEnable);
				timeout=0;
				Condition=buffer;
			}
		}
		break;
	}
	case batal:
	{
		LCD_SetCursor(0, 1);
		LCD_Print("Uang Kembali");LCD_SetCursor(13, 1);LCD_PrintNum(Coin_temp);LCD_Print("00");
		if(++i>30){
			i=0;
			Condition=finishdelay;
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
		LCD_SetCursor(0, 2);LCD_Print("Total: ");LCD_PrintNum(Jml_tunai);LCD_Print("00 ");
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

/*Led Configuration*/
void Led_Init(void)
{
	HAL_GPIO_WritePin(Led_0_GPIO_Port, Led_0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_3_GPIO_Port, Led_3_Pin, GPIO_PIN_SET);
}
void Led_Process(void)
{
	HAL_GPIO_WritePin(Led_0_GPIO_Port, Led_0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_3_GPIO_Port, Led_3_Pin, GPIO_PIN_RESET);
}
void Led_Cancel(void)
{
	HAL_GPIO_WritePin(Led_0_GPIO_Port, Led_0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_3_GPIO_Port, Led_3_Pin, GPIO_PIN_SET);
}
void Led_kembali500(void)
{
	HAL_GPIO_WritePin(Led_0_GPIO_Port, Led_0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_3_GPIO_Port, Led_3_Pin, GPIO_PIN_SET);
}
void Led_Kembali1000(void)
{
	HAL_GPIO_WritePin(Led_0_GPIO_Port, Led_0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_3_GPIO_Port, Led_3_Pin, GPIO_PIN_SET);
}
void Led_Kembali(void)
{
	HAL_GPIO_WritePin(Led_0_GPIO_Port, Led_0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_3_GPIO_Port, Led_3_Pin, GPIO_PIN_SET);
}
void Led_500(void)
{
	HAL_GPIO_WritePin(Led_0_GPIO_Port, Led_0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Led_1_GPIO_Port, Led_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_2_GPIO_Port, Led_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Led_3_GPIO_Port, Led_3_Pin, GPIO_PIN_SET);
}

