#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "ota_uart_fram.h"                  // Device header
#include "timer.h"
unsigned int tim_prescaler=72;
unsigned int tim_period=1000;
unsigned char blink = 0;
uint32_t timeout_cnt = 0;
void Timer_Config(void)
{
	TIM_TimeBaseInitTypeDef timer_struct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timer_struct.TIM_CounterMode		= TIM_CounterMode_Up;
	timer_struct.TIM_Prescaler			= 2000 - 1;
	timer_struct.TIM_Period				= 36000 - 1;
	timer_struct.TIM_ClockDivision		= 0;
	timer_struct.TIM_RepetitionCounter	= 0;
	TIM_TimeBaseInit(TIM2, &timer_struct);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)
	{		
		if(mode == 1)
		{
			timeout_cnt++;
			if(timeout_cnt == 15)
			{
				Flash_OTA_Error();
			}
		}
		else
		{
			timeout_cnt = 0;
		}
		
	}	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


