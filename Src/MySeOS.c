/*
 * MySeOS.c
 *
 *  Created on: Nov 22, 2019
 *      Author: izzat
 */
#include "main.h"
#include "MySeOS.h"
#include "MyTask.h"

void TIM4_IRQHandler(void)
{
	MyTask_Run();
}
void SCH_Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->PSC = 0;
	TIM4->ARR = 72;
	TIM4->CR1 |= TIM_CR1_URS;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->EGR |= TIM_EGR_UG;

	NVIC_EnableIRQ(TIM4_IRQn);
}
